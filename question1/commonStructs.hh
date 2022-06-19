#include <string>

struct User {
    int id;
    std::string username;
    std::string full_name;
    std::string profile_picture;
    bool followed;
};

struct Post {
    int id;
    std::string description;
    User owner;
    std::string image;
    int created_at;
    bool liked;
};

struct post_DB {
    int id;
    std::string description;
    int user_id;
    std::string image;
    int created_at;
};

struct user_DB {
    int id;
    std::string username;
    std::string email;
    std::string full_name;
    std::string profile_picture;
    std::string bio;
    int created_at;
};

struct like_DB {
    int id;
    int post_id;
    int user_id;
    int created_at;
};

struct follow_DB {
    int follower_id;
    int following_id;
    int created_at;
};