#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_USERS 10000

typedef struct Tweet {
    int tweetId;
    char message[100];
    struct Tweet* next;
} Tweet;

typedef struct User {
    int userId;
    char userName[50];
    int following[MAX_USERS];
    Tweet* tweets;
} User;

typedef struct Twitter {
    User users[MAX_USERS];
} Twitter;

typedef struct UserNode {
    User* user;
    struct UserNode* left;
    struct UserNode* right;
} UserNode;

UserNode* createUserNode(User* user) {
    UserNode* newNode = (UserNode*)malloc(sizeof(UserNode));
    newNode->user = user;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

UserNode* insertUserNode(UserNode* root, User* user) {
    if (root == NULL) {
        return createUserNode(user);
    }
    if (strcmp(user->userName, root->user->userName) < 0) {
        root->left = insertUserNode(root->left, user);
    } else {
        root->right = insertUserNode(root->right, user);
    }
    return root;
}

User* searchUser(UserNode* root, const char* userName) {
    if (root == NULL || strcmp(root->user->userName, userName) == 0) {
        return root ? root->user : NULL;
    }
    if (strcmp(userName, root->user->userName) < 0) {
        return searchUser(root->left, userName);
    } else {
        return searchUser(root->right, userName);
    }
}

Twitter* twitterCreate() {
    Twitter* obj = (Twitter*)malloc(sizeof(Twitter));
    for (int i = 0; i < MAX_USERS; i++) {
        obj->users[i].userId = i;
        obj->users[i].tweets = NULL;
        memset(obj->users[i].following, 0, sizeof(obj->users[i].following));
        obj->users[i].userName[0] = '\0';
    }
    return obj;
}

void twitterPostTweet(Twitter* obj, User* user, int tweetId, const char* message) {
    Tweet* newTweet = (Tweet*)malloc(sizeof(Tweet));
    newTweet->tweetId = tweetId%200000;
    strncpy(newTweet->message, message, sizeof(newTweet->message) - 1);
    newTweet->message[sizeof(newTweet->message) - 1] = '\0';
    newTweet->next = user->tweets;
    user->tweets = newTweet;
}



void followUser(Twitter* obj, int followerId, int followeeId) {
    if (followerId < 0 || followeeId < 0 || followerId >= MAX_USERS || followeeId >= MAX_USERS) {
        return;
    }
    obj->users[followerId].following[followeeId] = 1;
}

void unfollowUser(Twitter* obj, int followerId, int followeeId) {
    if (followerId < 0 || followeeId < 0 || followerId >= MAX_USERS || followeeId >= MAX_USERS) {
        return;
    }
    obj->users[followerId].following[followeeId] = 0;
}

void getNewsFeed(Twitter* obj, int userId, WINDOW* win) {
    if (userId < 0 || userId >= MAX_USERS) {
        mvwprintw(win, 3, 1, "Invalid user ID.");
        wrefresh(win);
        return;
    }

    mvwprintw(win, 3, 1, "Feed for User %d:", userId);
    int row = 4; // Start displaying tweets from row 4

    Tweet* stack[100]; 
    int top = -1; 

    // Collect tweets from the user and their followees
    for (int i = 0; i < MAX_USERS; i++) {
        if (i == userId || obj->users[userId].following[i]) {
            Tweet* t = obj->users[i].tweets;
            while (t) {
                stack[++top] = t;
                t = t->next;
            }
        }
    }

    // Display up to 10 tweets
    int count = 0;
    while (top >= 0 && count < 10) {
        Tweet* t = stack[top--];
        mvwprintw(win, row++, 1, "Tweet ID: %d, Message: %s", t->tweetId, t->message);
        count++;
    }

    if (count == 0) {
        mvwprintw(win, row++, 1, "No tweets available.");
    }

    wrefresh(win); // Refresh the window to show the feed
}


void freeUserTree(UserNode* root) {
    if (root == NULL) {
        return;
    }
    freeUserTree(root->left);
    freeUserTree(root->right);
    free(root);
}


void displayMenu(WINDOW* win) {
    wclear(win);
    mvwprintw(win, 1, 1, "Menu:");
    mvwprintw(win, 2, 1, "1. Post a tweet");
    mvwprintw(win, 3, 1, "2. Follow a user");
    mvwprintw(win, 4, 1, "3. Get feed");
    mvwprintw(win, 5, 1, "4. Unfollow a user");
    mvwprintw(win, 6, 1, "5. Search user by name");
    mvwprintw(win, 7, 1, "6. Exit the platform");
    mvwprintw(win, 9, 1, "Enter your choice: ");
    wrefresh(win);
}

int main() {
    Twitter* obj = twitterCreate();
    UserNode* userTree = NULL;

    // Initialize ncurses
    initscr();               // Start ncurses mode
    start_color();           // Enable color functionality
    echo();                  // Enable input echoing
    cbreak();                // Line buffering disabled, pass input to program
    keypad(stdscr, TRUE);    // Enable function keys

    // Initialize color pairs
    init_pair(1, COLOR_WHITE, COLOR_BLUE);     // White text on blue background
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);   // Yellow text on black background
    init_pair(3, COLOR_GREEN, COLOR_BLACK);    // Green text on black background
    init_pair(4, COLOR_RED, COLOR_BLACK);      // Red text on black background
    init_pair(5, COLOR_WHITE, COLOR_CYAN);     // White text on cyan background
    init_pair(6, COLOR_WHITE, COLOR_BLUE); // White text on light blue background
    init_pair(7, COLOR_YELLOW, COLOR_BLUE);    // Yellow text on blue background
    init_pair(8, COLOR_BLACK, COLOR_WHITE);    // Black text on white background
    init_pair(9, COLOR_WHITE, COLOR_MAGENTA);  // White text on magenta background
    init_pair(10, COLOR_WHITE, COLOR_GREEN);   // White text on green background
    init_pair(11, COLOR_CYAN, COLOR_BLACK);    // Cyan text on black background
    init_pair(12, COLOR_BLACK, COLOR_WHITE); 
    // Create menu window
    WINDOW* menuWin = newwin(50, 100, 1, 1);
       // White text on cyan background
     // Set cyan background for the window
 // Set background color for the window
    int choice;

    while (1) {
        wbkgd(menuWin, COLOR_PAIR(11));
        displayMenu(menuWin); // Displays the menu options
        wattron(menuWin, COLOR_PAIR(2)); // Yellow text for user input
        wrefresh(menuWin);
        wscanw(menuWin, "%d", &choice);
        wattroff(menuWin, COLOR_PAIR(2)); // Turn off yellow text

        switch (choice) {
        case 1: {
            int userId, tweetId;
            char userName[50], message[100];

            wattron(menuWin, COLOR_PAIR(3)); // Green text for input prompts
            mvwprintw(menuWin, 11, 1, "Enter user ID and username: ");
            wattroff(menuWin, COLOR_PAIR(3)); // Turn off green text
            wattron(menuWin, COLOR_PAIR(2)); // Yellow text for user input
            wrefresh(menuWin);
            wscanw(menuWin, "%d %s", &userId, userName);
            wattroff(menuWin, COLOR_PAIR(2)); // Turn off yellow text
            

            if (obj->users[userId].userName[0] == '\0') {
                strcpy(obj->users[userId].userName, userName);
                userTree = insertUserNode(userTree, &obj->users[userId]);
            }

            wattron(menuWin, COLOR_PAIR(3)); // Green text for tweet input
            mvwprintw(menuWin, 12, 1, "Enter tweet ID and message (1 word): ");
            wattroff(menuWin, COLOR_PAIR(3));
            wattron(menuWin, COLOR_PAIR(2)); // Yellow text for user input
            wrefresh(menuWin);
            wscanw(menuWin, "%d %s", &tweetId, message);
            wattroff(menuWin, COLOR_PAIR(2));   
            

            twitterPostTweet(obj, &obj->users[userId], tweetId, message);

            wattron(menuWin, COLOR_PAIR(4)); // Red text for success message
            mvwprintw(menuWin, 13, 1, "Tweet posted successfully!");
            mvwprintw(menuWin, 15, 1, "Press any key to return...");
            wrefresh(menuWin);
            getch();
            wattroff(menuWin, COLOR_PAIR(4));
            break;
        }
        case 2: {
            int followerId, followeeId;
            wattron(menuWin, COLOR_PAIR(2)); // Yellow text for user input
            mvwprintw(menuWin, 11, 1, "Enter follower ID and followee ID: ");
            wattroff(menuWin, COLOR_PAIR(2)); // Turn off yellow text
            wrefresh(menuWin);
            wattron(menuWin, COLOR_PAIR(10)); // while text for user input
            wscanw(menuWin, "%d %d", &followerId, &followeeId);
            wattroff(menuWin, COLOR_PAIR(10)); // Turn off white text
            followUser(obj, followerId, followeeId);

            wattron(menuWin, COLOR_PAIR(4)); // Red text for success message
            mvwprintw(menuWin, 13, 1, "Follow action completed!");
            mvwprintw(menuWin, 15, 1, "Press any key to return...");
            wrefresh(menuWin);
            getch();
            wattroff(menuWin, COLOR_PAIR(4));
            break;
        }
        case 3: {
            int userId;
            wattron(menuWin, COLOR_PAIR(2)); // Yellow text for user input
            mvwprintw(menuWin, 11, 1, "Enter user ID to get feed: ");
            wattroff(menuWin, COLOR_PAIR(2)); // Yellow text for user input
            wattron(menuWin, COLOR_PAIR(5)); // Yellow text for user input
            wrefresh(menuWin);
            wscanw(menuWin, "%d", &userId);
            wattroff(menuWin, COLOR_PAIR(2)); // Yellow text for user input

            wclear(menuWin);
            wattron(menuWin, COLOR_PAIR(1)); // Blue background for the feed
            getNewsFeed(obj, userId, menuWin);
            wrefresh(menuWin);
            wattroff(menuWin, COLOR_PAIR(1));
            mvwprintw(menuWin, 15, 1, "Press any key to return...");
            getch();
            break;
        }
        case 4: {
            int followerId, followeeId;
            wattron(menuWin, COLOR_PAIR(5)); // Yellow text for user input
            mvwprintw(menuWin, 11, 1, "Enter follower ID and followee ID: ");
            wattroff(menuWin, COLOR_PAIR(5)); // Yellow text for user input
            wrefresh(menuWin);
            wattron(menuWin, COLOR_PAIR(2)); // Yellow text for user input
            wscanw(menuWin, "%d %d", &followerId, &followeeId);
            wattroff(menuWin, COLOR_PAIR(2)); // Yellow text for user input
            unfollowUser(obj, followerId, followeeId);
            mvwprintw(menuWin, 13, 1, "Unfollow action completed!");
            mvwprintw(menuWin, 15, 1, "Press any key to return...");
            wrefresh(menuWin);
            getch();
            break;
        }
        case 5: {
            char userName[50];
            mvwprintw(menuWin, 11, 1, "Enter the username to search: ");
            wrefresh(menuWin);
            wattron(menuWin, COLOR_PAIR(9)); // Yellow text for user input
            wscanw(menuWin, "%s", userName);
            wattroff(menuWin, COLOR_PAIR(9)); // Turn off yellow text

            User* user = searchUser(userTree, userName);
            wclear(menuWin);
            if (user) {
                mvwprintw(menuWin, 1, 1, "User found: %s ID: %d", user->userName, user->userId);
                mvwprintw(menuWin, 3, 1, "Tweets by %s:", user->userName);

                Tweet* t = user->tweets;
                int y = 5;
                while (t) {
                    mvwprintw(menuWin, y++, 1, "Tweet ID: %d, Message: %s", t->tweetId, t->message);
                    t = t->next;
                }
            } else {
                mvwprintw(menuWin, 1, 1, "User not found.");
            }
            mvwprintw(menuWin, 15, 1, "Press any key to return...");
            wrefresh(menuWin);
            getch();
            break;
        }
        case 6: {
            freeUserTree(userTree);
            free(obj);
            delwin(menuWin);
            endwin();
            return 0;
        }
        default:
            mvwprintw(menuWin, 9, 1, "Invalid choice! Try again.");
            wrefresh(menuWin);
            getch();
        }
    }
    return 0;
}
