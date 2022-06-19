#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>

struct Post {
    int id;
    std::string description;
    std::string image;
    int created_at;
};

struct PriorityQueueItem {
    Post post;
    int listIndex;
    int itemIndex;
};

//Create Proper PriorityQueueItem from given post list
std::pair<bool, PriorityQueueItem> createProperQueueItem(const std::unordered_set<int>& usedPostIds, 
        const std::vector<Post>& posts, int listIndex, int itemIndex) {

    std::pair<bool, PriorityQueueItem> result = std::make_pair(false, PriorityQueueItem{});
    bool isItemFound{false};
    PriorityQueueItem pqItem;
    for(int i = itemIndex; i>=0; i--) {
        if(usedPostIds.count(posts[i].id) == 0) {
            pqItem.post = posts[i];
            pqItem.listIndex = listIndex;
            pqItem.itemIndex = i;
            isItemFound = true;
            break;
        }
    }

    if(isItemFound) {
        result.first = true;
        result.second = pqItem;
    }

    return result;
}

using my_value_t = PriorityQueueItem;
using my_container_t = std::vector<my_value_t>;

// The main logic for merging posts
// The time complexity of main algorithm(The while statements) is : O(N*logk)
//     - Pop and emplace operations for priority queue are in O(logk)
//     - They are performed N times,
//     - So, it is O(N*logk)
// The complexity of merge_posts algorithm is in O(M+(N*logk))
std::vector<Post> merge_posts(const std::vector<std::vector<Post>>& list_of_posts) {
    std::vector<Post> mergedPosts;

    auto my_comp = [](const my_value_t& z1, const my_value_t& z2) {
        bool result{false};
        if(z2.post.created_at > z1.post.created_at) {
            result = true;
        } else if(z2.post.created_at == z1.post.created_at) {
            if(z2.post.id > z1.post.id) {
                result = true;
            } else {
                result = false;
            }
        } else {
            result = false;
        }

        return result;
    };

    // The container is used while merging the multiple post lists.
    std::priority_queue<my_value_t, my_container_t, decltype(my_comp)> priorityQueue{my_comp};
    //This variable is used to distiguish the lists in list_of_post 
    int listIndex{0};
    // This container keeps the used post ids to determine if the posts are unique.
    std::unordered_set<int> usedPost;

    // Add one item from each list
    // Time Comlexity is in O(M)
    for(const auto& posts : list_of_posts) {
        auto res = createProperQueueItem(usedPost, posts, listIndex, posts.size()-1);
        if(res.first) {
            usedPost.emplace(res.second.post.id);
            listIndex++;
            priorityQueue.emplace(std::move(res.second));
        }
    }

    // The main algorithm logic for merging multiple post list
    // The result list contains posts which are sorted by created_at attribute in descending order
    // For post that have same created_at attrinbute, they are sorted by id attribute in descending order
    // The result contains the unique posts
    // The uniqueness is determined by id attribute
    // Time Comlexity is in O(N*logk)
    while(!priorityQueue.empty()) {
        auto temp = priorityQueue.top();
        priorityQueue.pop();
        mergedPosts.emplace_back(temp.post);
        auto res = createProperQueueItem(usedPost, list_of_posts[temp.listIndex], temp.listIndex, temp.itemIndex-1);
        if(res.first) {
            usedPost.emplace(res.second.post.id);
            priorityQueue.emplace(std::move(res.second));
        }
    }

    return mergedPosts;

}

int main()
{
    // Test vector to verify my algorithm
    std::vector<std::vector<Post>> list_of_posts{
        {
            {1, "desc1", "image1", 1},
            {3, "desc3", "image3", 3},
            {5, "desc5", "image5", 5}
        },
        {
            {1, "desc1", "image1", 1},
            {2, "desc2", "image2", 1}
        },
        {
            {2, "desc2", "image1", 2},
            {4, "desc4", "image4", 7},
            {5, "desc5", "image5", 5}
        },
        {
            {6, "desc6", "image6", 5},
            {8, "desc8", "image8", 5},
            {7, "desc7", "image7", 6}
        },
    };

    auto mergedPosts{std::move(merge_posts(list_of_posts))};
    
    std::cout << "-----MergedPosts------" << std::endl;
    for(const auto& elem : mergedPosts) {
        std::cout << "Post id :" << elem.id << ", created_at : " << elem.created_at << std::endl;
    }

    return 0;
}