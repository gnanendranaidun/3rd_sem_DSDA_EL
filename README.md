# TUI-Twitter: A Terminal-Based Twitter Replica

## Overview
TUI-Twitter is a simple Twitter-like application built using C and `ncurses`. It provides a terminal-based user interface for tweeting, following/unfollowing users, and viewing a news feed. The project demonstrates the use of data structures like linked lists and binary search trees while leveraging `ncurses` for interactive UI rendering.

## Features
- **User Registration & Login**: Create and manage user accounts.
- **Tweeting**: Post tweets with character limits.
- **Follow/Unfollow Users**: Manage connections between users.
- **News Feed**: View tweets from followed users.
- **Ncurses-Based UI**: Interactive TUI with color support.
- **Efficient Data Storage**: Uses linked lists and binary search trees.

## Installation
### Prerequisites
- A Linux or macOS terminal environment.
- `ncurses` library installed. If not installed, install it using:
  ```sh
  sudo apt-get install libncurses5-dev  # Debian-based systems
  sudo pacman -S ncurses               # Arch-based systems
  brew install ncurses                 # macOS (Homebrew)
  ```

### Compilation
To compile the project, use `gcc`:
```sh
gcc -o twitter twitter.c -lncurses
```

### Running the Application
```sh
./twitter
```

## Usage
Once launched, the interface provides options for:
- Registering/Login
- Posting Tweets
- Following Users
- Viewing Tweets from Followed Users
- Exiting the application

Use keyboard navigation to move through the menu and interact with the system.

## File Structure
```
|--twitter.c             # main file for gcc
|--twitter.exec          # Unix executable file
|-- README.md            # Project documentation
|-- Makefile             # Build automation
```

## Future Enhancements
- **Hashtags & Trending Topics**
- **Direct Messages**
- **Retweets & Likes**
- **Persistent Storage (File or Database)**

## Contributing
Feel free to fork the repository, submit issues, and contribute enhancements!

## License
This project is open-source.

---
### Author
Gnanendra Naidu N 🚀

