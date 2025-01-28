#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_FRIENDS = 50;
const int MAX_POSTS = 20;

class User {
public:
    string email;
    string password;
    string name;
    string friends[MAX_FRIENDS];
    string posts[MAX_POSTS];
    int friendCount;
    int postCount;

    User() : email(""), password(""), name(""), friendCount(0), postCount(0) {}

    User(const string& email, const string& password, const string& name) : email(email), password(password), name(name), friendCount(0), postCount(0) {}

    void addFriend(const string& friendName) {
        if (friendCount < MAX_FRIENDS) {
            friends[friendCount++] = friendName;
            cout << friendName << " added as a friend successfully!\n";
        }
        else {
            cerr << "Error: Maximum friend limit reached.\n";
        }
    }

    void addPost(const string& post) {
        if (postCount < MAX_POSTS) {
            posts[postCount++] = post;
            cout << "Post added successfully!\n";
        }
        else {
            cerr << "Error: Maximum post limit reached.\n";
        }
    }

    void displayPosts() const {
        if (postCount == 0) {
            cout << "No posts available.\n";
            return;
        }

        cout << "Posts by " << name << " (" << email << "):\n";
        for (int i = 0; i < postCount; ++i) {
            cout << i + 1 << ". " << posts[i] << endl;
        }
    }

    void displayFriends() const {
        if (friendCount == 0) {
            cout << "No friends found.\n";
            return;
        }

        cout << "Friends of " << name << ":\n";
        for (int i = 0; i < friendCount; ++i) {
            cout << "- " << friends[i] << endl;
        }
    }
};

class SocialNetwork {
private:
    User users[100];
    int userCount;
    User* loggedInUser;

public:
    SocialNetwork() : userCount(0), loggedInUser(nullptr) {}

    void addUser(const string& email, const string& password, const string& name) {
        if (userCount >= 100) {
            cerr << "Error: Maximum user limit reached.\n";
            return;
        }

        for (int i = 0; i < userCount; ++i) {
            if (users[i].email == email) {
                cerr << "Error: User with this email already exists.\n";
                return;
            }
        }

        users[userCount++] = User(email, password, name);

        ofstream userFile("users.txt", ios::app);
        if (userFile.is_open()) {
            userFile << email << "," << password << "," << name << endl;
            userFile.close();
            cout << "User added successfully!\n";
        }
        else {
            cerr << "Error: Unable to open file for writing.\n";
        }
    }

    void displayUsers() {
        if (userCount == 0) {
            cout << "No users found.\n";
            return;
        }

        cout << "Users:\n";
        for (int i = 0; i < userCount; ++i) {
            cout << i + 1 << ". " << users[i].name << " - " << users[i].email << endl;
        }
    }

    void login(const string& email, const string& password) {
        for (int i = 0; i < userCount; ++i) {
            if (users[i].email == email && users[i].password == password) {
                loggedInUser = &users[i];
                cout << "Login successful! Welcome, " << loggedInUser->name << "!\n";
                return;
            }
        }

        cerr << "Error: Invalid email or password.\n";
    }

    void addFriend(const string& friendName) {
        if (!loggedInUser) {
            cerr << "Error: No user is logged in.\n";
            return;
        }

        for (int i = 0; i < userCount; ++i) {
            if (users[i].name == friendName && &users[i] != loggedInUser) {
                loggedInUser->addFriend(friendName);

                ofstream friendFile("friends.txt", ios::app);
                if (friendFile.is_open()) {
                    friendFile << loggedInUser->name << " -> " << friendName << endl;
                    friendFile.close();
                }
                else {
                    cerr << "Error: Unable to open friends file for writing.\n";
                }

                return;
            }
        }

        cerr << "Error: User with the name \"" << friendName << "\" not found.\n";
    }

    void displayFriends() {
        if (!loggedInUser) {
            cerr << "Error: No user is logged in.\n";
            return;
        }

        loggedInUser->displayFriends();
    }

    void addPost(const string& content) {
        if (!loggedInUser) {
            cerr << "Error: No user is logged in.\n";
            return;
        }

        loggedInUser->addPost(content);

        ofstream postFile("posts.txt", ios::app);
        if (postFile.is_open()) {
            postFile << loggedInUser->name << " (" << loggedInUser->email << "): " << content << endl;
            postFile.close();
            cout << "Post saved to file.\n";
        }
        else {
            cerr << "Error: Unable to open posts file for writing.\n";
        }
    }

    void displayMyPosts() {
        if (!loggedInUser) {
            cerr << "Error: No user is logged in.\n";
            return;
        }

        loggedInUser->displayPosts();
    }

    void loadUsers() {
        ifstream userFile("users.txt");
        if (!userFile.is_open()) {
            cerr << "Error: Unable to open file for reading. Starting with no users.\n";
            return;
        }

        string line;
        while (getline(userFile, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);

            if (pos1 == string::npos || pos2 == string::npos) continue;

            string email = line.substr(0, pos1);
            string password = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string name = line.substr(pos2 + 1);

            users[userCount++] = User(email, password, name);
        }

        userFile.close();
        cout << "Loaded " << userCount << " user(s) from file.\n";
    }

    void logout() {
        if (loggedInUser) {
            cout << "Logout successful! Goodbye, " << loggedInUser->name << "!\n";
            loggedInUser = nullptr;
        }
        else {
            cerr << "Error: No user is logged in.\n";
        }
    }
};

int main() {
    SocialNetwork network;
    network.loadUsers();
    int choice;
    string email, password, name, postContent, friendName;

    while (true) {
        cout << "\nSocial Network Menu:\n";
        cout << "1. Add User\n2. Display Users\n3. Add Friend\n4. Display Friends\n5. Login\n6. Add Post\n7. Display My Posts\n8. Logout\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            network.addUser(email, password, name);
            break;
        case 2:
            network.displayUsers();
            break;
        case 3:
            cout << "Enter Friend's Name: ";
            cin.ignore();
            getline(cin, friendName);
            network.addFriend(friendName);
            break;
        case 4:
            network.displayFriends();
            break;
        case 5:
            cout << "Enter Email: ";
            cin >> email;
            cout << "Enter Password: ";
            cin >> password;
            network.login(email, password);
            break;
        case 6:
            cout << "Enter your post content: ";
            cin.ignore();
            getline(cin, postContent);
            network.addPost(postContent);
            break;
        case 7:
            network.displayMyPosts();
            break;
        case 8:
            network.logout();
            break;
        case 9:
            cout << "Exiting...\n";
            return 0;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
