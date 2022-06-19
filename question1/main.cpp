#include <iostream>
#include <vector>

#include "commonStructs.hh"

// This function is used to indicate the followed attribute in result list
bool getFollowed(int follower_id, int following_id) {
    bool followed{true};

    followed = follower_id == 2 && following_id == 3 ? true : false;

    // Followed variable will be provided by using the following query
    // db_followed = SELECT COUNT(1) FROM follow where follower_id = follower_id and following_id = following_id
    // If db_followed returns 1, then followed variable will remain as is. Otherwise it will turn to false.

    return followed;
}

// This function is used to indicate the liked attribute in result list
bool getLiked(int post_id, int requesting_user_id) {
    bool liked{true};

    liked = post_id == 1 && requesting_user_id == 2 ? true : false; 
    // Liked variable will be provided by using the following query
    // db_liked = SELECT COUNT(1) FROM like where post_id = post_id and user_id = requesting_user_id
    // If db_liked returns 1, then liked variable will remain as is. Otherwise it will turn to false.

    return liked;
}

// This function is used to fetch the corresponding user record from DB according to given user id
struct user_DB getUserFromDB(int user_id) {
    struct user_DB user;

    user_DB mockUser;
    switch(user_id) {
        case 1:
            mockUser.id = 1;
            mockUser.username = "user1";
            mockUser.full_name = "user1FullName";
            mockUser.profile_picture = "user1PP";
        break;
        case 2:
            mockUser.id = 2;
            mockUser.username = "user2";
            mockUser.full_name = "user2FullName";
            mockUser.profile_picture = "user2PP";
        break;
        case 3:
            mockUser.id = 3;
            mockUser.username = "user3";
            mockUser.full_name = "user3FullName";
            mockUser.profile_picture = "user3PP";
        break;
        default:
            mockUser.id = 4;
            mockUser.username = "user4";
            mockUser.full_name = "user4FullName";
            mockUser.profile_picture = "user4PP";
        break;
    }
    // The user recod will be fetched bu using the following query.
    // db_user = SELECT id, username, full_name, profile_picture FROM user where id = user_id
    // Then create struct user_DB from db_user

    user = mockUser;

    return user;
}

// This function is used to fetch the corresponding post record from DB according to given post ids
std::vector<post_DB> getPostsFromDB(const std::vector<int>& post_ids) {
    std::vector<post_DB> db_posts;

    std::vector<post_DB> mockPostDB{
        {1, "desc1", 1, "image1", 1}, 
        {2, "desc2", 2, "image2", 2}, 
        {3, "desc3", 3, "image3", 3}, 
        {4, "desc4", 4, "image4", 4}};

    db_posts = mockPostDB;

    std::string postIdsStrForDb{"("};
    if(!post_ids.empty()) {
        postIdsStrForDb += std::to_string(post_ids[0]);
    }
    for(int i=1; i<post_ids.size(); i++) {
        postIdsStrForDb += ", " + std::to_string(post_ids[i]);
    }

    postIdsStrForDb += ")";

    //Posts are fetched from DB by using the following sql query:
    // db_posts = SELECT * FROM post WHERE id IN postIdsStrForDb

    return db_posts;
}

// This function is used to create owner attribute in result list
struct User createOwner(const user_DB& db_user, int requesting_user_id) {
    User owner;
    owner.id = db_user.id;
    owner.username = db_user.username;
    owner.full_name = db_user.full_name;
    owner.profile_picture = db_user.profile_picture;
    owner.followed = getFollowed(requesting_user_id, db_user.id);
    return owner;

}

// The main function which gets posts with requested sturct type 
std::vector<Post> get_posts(const std::vector<int>& post_ids, int requesting_user_id) {
    std::vector<Post> result;

    // Firstly, all corresponding posts are fetched from DB according to given post_ids
    std::vector<post_DB> db_posts{std::move(getPostsFromDB(post_ids))};

    Post post;
    user_DB db_user;
    for(const auto& db_post : db_posts) {
        post.id = db_post.id;
        post.description = db_post.description;
        post.image = db_post.image;
        post.created_at = db_post.created_at;
        // Get user record from DB
        db_user = getUserFromDB(db_post.user_id);
        // Create owner struct by using previously fethced user record 
        post.owner = std::move(createOwner(db_user, requesting_user_id));
        // Fill the liked field for corresponding post
        post.liked = getLiked(post.id, requesting_user_id);
        result.emplace_back(post);
    }

    return result;

}

void printUser(const User& user) {
    std::cout << "---- PRINT USER BEGIN -----" << std::endl;
    std::cout << "id : " << user.id << std::endl;
    std::cout << "username : " << user.username << std::endl;
    std::cout << "fullname : " << user.full_name << std::endl;
    std::cout << "pp : " << user.profile_picture << std::endl;
    std::cout << "followed : " << user.followed << std::endl;
    std::cout << "---- PRINT USER END -----" << std::endl;
}

void printPost(const Post& post) {
    std::cout << "---- PRINT POST BEGIN -----" << std::endl;
    std::cout << "id : " << post.id << std::endl;
    std::cout << "desc : " << post.description << std::endl;
    std::cout << "image : " << post.image << std::endl;
    std::cout << "created_at : " << post.created_at << std::endl;
    std::cout << "owner : " << std::endl;
    printUser(post.owner);
    std::cout << "liked :" << post.liked << std::endl;
    std::cout << "---- PRINT POST END -----" << std::endl;
}

int main()
{
    std::vector<int> post_ids{1,3,4};
    int requesting_user_id{2};
    std::vector<Post> posts{std::move(get_posts(post_ids, requesting_user_id))};
    for(const auto& elem : posts) {
        printPost(elem);
    }
    
    return 0;
}