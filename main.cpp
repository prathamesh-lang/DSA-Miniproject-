/**
 * ============================================================================
 * Project Title : Music Playlist Management System
 * Data Structure: Doubly Linked List
 * Technology    : C++ (C++17 Standard)
 * Authors       : Prathamesh Patil (Roll No. 48, PRN: U251A189)
 *                 Omkar Rane (Roll No. 50, PRN: U251A194)
 * Guide         : Prof. Vaishali Dhawas
 * Department    : Department of Computer Engineering
 * Institute     : Nutan Maharashtra Institute of Engineering & Technology, Pune
 * Academic Year : 2026-2027
 * ============================================================================
 * 
 * Description:
 * This program implements an interactive, menu-driven Music Playlist Management
 * System using a Doubly Linked List. Each song is stored as an independently
 * allocated node containing song ID, title, artist, and duration, linked
 * bidirectionally using 'prev' and 'next' pointers. Three list-level pointers
 * are maintained:
 *   - head    : Points to the first song in the playlist.
 *   - tail    : Points to the last song in the playlist.
 *   - current : Points to the currently active / playing song.
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>

// ============================================================================
// 1. DATA STRUCTURE: SONG NODE
// ============================================================================
struct Song {
    int songID;             // Unique identifier for the song
    std::string title;      // Song title
    std::string artist;     // Artist / Band name
    int duration;           // Duration in seconds
    Song* prev;             // Pointer to the previous song in the playlist
    Song* next;             // Pointer to the next song in the playlist

    // Constructor to initialise a new song node
    Song(int id, const std::string& t, const std::string& a, int d)
        : songID(id), title(t), artist(a), duration(d), prev(nullptr), next(nullptr) {}
};

// ============================================================================
// 2. HELPER UTILITIES: INPUT VALIDATION & STRING FORMATTING
// ============================================================================

/**
 * Trims leading and trailing whitespace from a string.
 */
static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

/**
 * Converts a string to lowercase for case-insensitive comparisons.
 */
static std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return result;
}

/**
 * Formats duration in seconds into a friendly mm:ss string.
 */
static std::string formatDuration(int totalSeconds) {
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    std::ostringstream oss;
    oss << minutes << "m " << (seconds < 10 ? "0" : "") << seconds << "s (" << totalSeconds << "s)";
    return oss.str();
}

/**
 * Reads a non-empty string safely from std::cin using std::getline.
 */
static std::string readString(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, input)) {
            std::cin.clear();
            return "";
        }
        input = trim(input);
        if (!input.empty()) {
            return input;
        }
        std::cout << "  [!] Input cannot be empty. Please try again.\n";
    }
}

/**
 * Reads an integer safely from std::cin, rejecting letters, symbols, or invalid numbers.
 */
static int readInteger(const std::string& prompt) {
    std::string line;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, line)) {
            std::cin.clear();
            continue;
        }
        line = trim(line);
        if (line.empty()) {
            std::cout << "  [!] Input cannot be empty. Please enter a valid number.\n";
            continue;
        }

        std::istringstream iss(line);
        int value;
        char extra;
        if ((iss >> value) && !(iss >> extra)) {
            return value;
        }
        std::cout << "  [!] Invalid numeric input. Please enter a valid integer.\n";
    }
}

// ============================================================================
// 3. PLAYLIST MANAGER CLASS (DOUBLY LINKED LIST)
// ============================================================================
class Playlist {
private:
    Song* head;      // Pointer to first node (prev == nullptr)
    Song* tail;      // Pointer to last node  (next == nullptr)
    Song* current;   // Pointer to currently selected / playing node

public:
    // Constructor: Initialises an empty doubly linked list
    Playlist() : head(nullptr), tail(nullptr), current(nullptr) {}

    // Destructor: Cleans up all dynamically allocated memory
    ~Playlist() {
        clear();
    }

    // Disable copy constructor and copy assignment to prevent shallow copy issues
    Playlist(const Playlist&) = delete;
    Playlist& operator=(const Playlist&) = delete;

    /**
     * Checks if the playlist contains any songs.
     */
    bool isEmpty() const {
        return head == nullptr;
    }

    /**
     * Helper to search for a song node by ID.
     * Returns a pointer to the matching Song, or nullptr if not found.
     * Time Complexity: O(n)
     */
    Song* findSongByID(int id) const {
        Song* temp = head;
        while (temp != nullptr) {
            if (temp->songID == id) {
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    /**
     * 1. ADD SONG
     * Appends a new song to the end of the doubly linked list.
     * Handles:
     *   - Duplicate ID rejection.
     *   - Non-positive duration rejection.
     *   - Empty playlist (head, tail, and current point to the new node).
     *   - Non-empty playlist (appended to tail; current pointer preserved).
     * Time Complexity: O(1) appending (plus O(n) check for unique ID).
     */
    void addSong() {
        std::cout << "\n----------------------------------------\n";
        std::cout << "               ADD SONG                 \n";
        std::cout << "----------------------------------------\n";

        int id = readInteger("Enter Song ID (positive integer): ");
        if (id <= 0) {
            std::cout << "\n[Error] Song ID must be a positive integer greater than 0.\n";
            return;
        }

        // Verify duplicate ID
        if (findSongByID(id) != nullptr) {
            std::cout << "\n[Error] Song with ID " << id << " already exists in the playlist.\n";
            std::cout << "        Each song must have a unique ID. Operation cancelled.\n";
            return;
        }

        std::string title = readString("Enter Song Title: ");
        std::string artist = readString("Enter Artist Name: ");
        int duration = readInteger("Enter Duration in seconds: ");

        if (duration <= 0) {
            std::cout << "\n[Error] Duration must be a positive integer greater than 0.\n";
            return;
        }

        // Allocate the new song node dynamically
        Song* newNode = new Song(id, title, artist, duration);

        if (head == nullptr) {
            // Case 1: First song added to an empty playlist
            head = newNode;
            tail = newNode;
            current = newNode; // First song automatically becomes current
            std::cout << "\n[Success] Added first song \"" << title << "\" to the playlist!\n";
            std::cout << "          Current song is now set to this song.\n";
        } else {
            // Case 2: Append to the end of existing playlist
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
            // Preserves existing current pointer selection
            std::cout << "\n[Success] Added \"" << title << "\" to the end of the playlist.\n";
        }
    }

    /**
     * 2. DELETE SONG
     * Searches for a song by ID and removes it from the doubly linked list.
     * Handles all edge cases:
     *   - Empty playlist.
     *   - Song ID not found.
     *   - Deleting the only node in the list.
     *   - Deleting the head node.
     *   - Deleting the tail node.
     *   - Deleting a middle node.
     *   - Deleting the currently selected song (reassigns current safely).
     * Time Complexity: O(n) to locate node, O(1) to relink pointers.
     */
    void deleteSong() {
        std::cout << "\n----------------------------------------\n";
        std::cout << "              DELETE SONG               \n";
        std::cout << "----------------------------------------\n";

        if (isEmpty()) {
            std::cout << "[Info] Playlist is empty. There are no songs to delete.\n";
            return;
        }

        int id = readInteger("Enter Song ID to delete: ");
        Song* delNode = findSongByID(id);

        if (delNode == nullptr) {
            std::cout << "\n[Error] Song with ID " << id << " not found in the playlist.\n";
            return;
        }

        std::string deletedTitle = delNode->title;
        bool wasCurrent = (delNode == current);

        // Update 'current' pointer if the deleted node is the active song
        if (wasCurrent) {
            if (delNode->next != nullptr) {
                // Next remaining song available
                current = delNode->next;
            } else if (delNode->prev != nullptr) {
                // Otherwise fallback to previous remaining song
                current = delNode->prev;
            } else {
                // List will become empty
                current = nullptr;
            }
        }

        // Relink neighbouring pointers
        if (delNode == head && delNode == tail) {
            // Case A: Deleting the ONLY node in the playlist
            head = nullptr;
            tail = nullptr;
            current = nullptr;
        } else if (delNode == head) {
            // Case B: Deleting the FIRST (head) node
            head = head->next;
            head->prev = nullptr;
        } else if (delNode == tail) {
            // Case C: Deleting the LAST (tail) node
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            // Case D: Deleting a MIDDLE node
            delNode->prev->next = delNode->next;
            delNode->next->prev = delNode->prev;
        }

        // Free the allocated memory
        delete delNode;

        std::cout << "\n[Success] Song \"" << deletedTitle << "\" (ID: " << id << ") was removed from the playlist.\n";
        if (isEmpty()) {
            std::cout << "          The playlist is now completely empty.\n";
        } else if (wasCurrent) {
            std::cout << "          Active song updated to: \"" << current->title << "\" (ID: " << current->songID << ").\n";
        }
    }

    /**
     * 3. SEARCH SONG
     * Provides options to search either by unique Song ID or by Song Title.
     * Features:
     *   - Case-insensitive matching for song titles.
     *   - Safe handling of nonexistent songs.
     * Time Complexity: O(n)
     */
    void searchSong() const {
        std::cout << "\n----------------------------------------\n";
        std::cout << "              SEARCH SONG               \n";
        std::cout << "----------------------------------------\n";

        if (isEmpty()) {
            std::cout << "[Info] Playlist is empty. There are no songs to search.\n";
            return;
        }

        std::cout << "1. Search by Song ID\n";
        std::cout << "2. Search by Song Title\n";
        int choice = readInteger("Enter search option (1-2): ");

        if (choice == 1) {
            int id = readInteger("Enter Song ID to search: ");
            Song* match = findSongByID(id);
            if (match != nullptr) {
                std::cout << "\n[Found] Song Details:\n";
                std::cout << "  --------------------------------------------------\n";
                std::cout << "  Song ID   : " << match->songID << "\n";
                std::cout << "  Title     : " << match->title << "\n";
                std::cout << "  Artist    : " << match->artist << "\n";
                std::cout << "  Duration  : " << formatDuration(match->duration) << "\n";
                std::cout << "  Status    : " << (match == current ? "-> Currently Playing" : "Queued") << "\n";
                std::cout << "  --------------------------------------------------\n";
            } else {
                std::cout << "\n[Result] No song found with ID " << id << ".\n";
            }
        } else if (choice == 2) {
            std::string searchTitle = readString("Enter Song Title to search: ");
            std::string lowerQuery = toLower(searchTitle);

            Song* temp = head;
            int matchCount = 0;

            while (temp != nullptr) {
                if (toLower(temp->title) == lowerQuery) {
                    if (matchCount == 0) {
                        std::cout << "\n[Found] Matching Song(s):\n";
                        std::cout << "  --------------------------------------------------\n";
                    }
                    matchCount++;
                    std::cout << "  Match #" << matchCount << ":\n";
                    std::cout << "    Song ID   : " << temp->songID << "\n";
                    std::cout << "    Title     : " << temp->title << "\n";
                    std::cout << "    Artist    : " << temp->artist << "\n";
                    std::cout << "    Duration  : " << formatDuration(temp->duration) << "\n";
                    std::cout << "    Status    : " << (temp == current ? "-> Currently Playing" : "Queued") << "\n";
                }
                temp = temp->next;
            }

            if (matchCount > 0) {
                std::cout << "  --------------------------------------------------\n";
            } else {
                std::cout << "\n[Result] No song found matching title \"" << searchTitle << "\".\n";
            }
        } else {
            std::cout << "\n[Error] Invalid search option. Please choose 1 or 2.\n";
        }
    }

    /**
     * 4. DISPLAY PLAYLIST
     * Traverses the doubly linked list from head to tail and prints each song.
     * Highlights the currently playing song with a marker '-> [CURRENT]'.
     * Displays summary statistics (total songs, total duration).
     * Time Complexity: O(n)
     */
    void displayPlaylist() const {
        std::cout << "\n========================================================================================\n";
        std::cout << "                                  CURRENT PLAYLIST                                      \n";
        std::cout << "========================================================================================\n";

        if (isEmpty()) {
            std::cout << "[Info] The playlist is currently empty.\n";
            std::cout << "       Use Option 1 from the main menu to add your favorite songs!\n";
            std::cout << "========================================================================================\n";
            return;
        }

        std::cout << std::left
                  << std::setw(6)  << "No."
                  << std::setw(10) << "Song ID"
                  << std::setw(28) << "Title"
                  << std::setw(22) << "Artist"
                  << std::setw(16) << "Duration"
                  << "Status" << "\n";
        std::cout << "----------------------------------------------------------------------------------------\n";

        Song* temp = head;
        int index = 1;
        int totalSeconds = 0;

        while (temp != nullptr) {
            std::string statusMarker = (temp == current) ? "-> [CURRENT]" : "";
            std::string durStr = formatDuration(temp->duration);

            std::cout << std::left
                      << std::setw(6)  << index
                      << std::setw(10) << temp->songID
                      << std::setw(28) << temp->title
                      << std::setw(22) << temp->artist
                      << std::setw(16) << durStr
                      << statusMarker << "\n";

            totalSeconds += temp->duration;
            temp = temp->next;
            index++;
        }

        std::cout << "----------------------------------------------------------------------------------------\n";
        std::cout << "Total Songs     : " << (index - 1) << "\n";
        std::cout << "Total Duration  : " << formatDuration(totalSeconds) << "\n";
        if (current != nullptr) {
            std::cout << "Currently Active: \"" << current->title << "\" by " << current->artist 
                      << " (ID: " << current->songID << ")\n";
        } else {
            std::cout << "Currently Active: None\n";
        }
        std::cout << "========================================================================================\n";
    }

    /**
     * 5. NEXT SONG
     * Moves the 'current' pointer forward to current->next.
     * Boundary check: If already at tail, displays boundary notice and keeps selection.
     * Non-circular navigation.
     * Time Complexity: O(1)
     */
    void nextSong() {
        std::cout << "\n----------------------------------------\n";
        std::cout << "               NEXT SONG                \n";
        std::cout << "----------------------------------------\n";

        if (isEmpty() || current == nullptr) {
            std::cout << "[Info] Playlist is empty. There is no song to play.\n";
            return;
        }

        if (current->next != nullptr) {
            // Advance current pointer to the next song node
            current = current->next;
            std::cout << "[Action] Moved forward to the next song.\n";
            std::cout << "         Now Playing: \"" << current->title << "\" by " << current->artist 
                      << " [" << formatDuration(current->duration) << "]\n";
        } else {
            // Already at the last node (tail)
            std::cout << "[Boundary] You are already at the last song in the playlist.\n";
            std::cout << "           Cannot move forward. Still playing: \"" << current->title << "\".\n";
        }
    }

    /**
     * 6. PREVIOUS SONG
     * Moves the 'current' pointer backward to current->prev.
     * Boundary check: If already at head, displays boundary notice and keeps selection.
     * Non-circular navigation.
     * Time Complexity: O(1)
     */
    void previousSong() {
        std::cout << "\n----------------------------------------\n";
        std::cout << "             PREVIOUS SONG              \n";
        std::cout << "----------------------------------------\n";

        if (isEmpty() || current == nullptr) {
            std::cout << "[Info] Playlist is empty. There is no song to play.\n";
            return;
        }

        if (current->prev != nullptr) {
            // Move current pointer back to the previous song node
            current = current->prev;
            std::cout << "[Action] Moved backward to the previous song.\n";
            std::cout << "         Now Playing: \"" << current->title << "\" by " << current->artist 
                      << " [" << formatDuration(current->duration) << "]\n";
        } else {
            // Already at the first node (head)
            std::cout << "[Boundary] You are already at the first song in the playlist.\n";
            std::cout << "           Cannot move backward. Still playing: \"" << current->title << "\".\n";
        }
    }

    /**
     * 7. CLEANUP & MEMORY MANAGEMENT
     * Safely traverses the entire doubly linked list from head,
     * deletes each node, and resets head, tail, and current to nullptr.
     * Time Complexity: O(n)
     */
    void clear() {
        Song* currentSong = head;
        while (currentSong != nullptr) {
            Song* nextSong = currentSong->next;
            delete currentSong;
            currentSong = nextSong;
        }
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }
};

// ============================================================================
// 4. MAIN USER INTERFACE & MENU LOOP
// ============================================================================
int main() {
    Playlist playlist;
    bool running = true;

    while (running) {
        std::cout << "\n";
        std::cout << "========================================\n";
        std::cout << "       MUSIC PLAYLIST MANAGEMENT        \n";
        std::cout << "========================================\n";
        std::cout << "1. Add Song\n";
        std::cout << "2. Delete Song\n";
        std::cout << "3. Search Song\n";
        std::cout << "4. Display Playlist\n";
        std::cout << "5. Next Song\n";
        std::cout << "6. Previous Song\n";
        std::cout << "7. Exit\n";
        std::cout << "========================================\n";

        int choice = readInteger("Enter your choice: ");

        switch (choice) {
            case 1:
                playlist.addSong();
                break;
            case 2:
                playlist.deleteSong();
                break;
            case 3:
                playlist.searchSong();
                break;
            case 4:
                playlist.displayPlaylist();
                break;
            case 5:
                playlist.nextSong();
                break;
            case 6:
                playlist.previousSong();
                break;
            case 7: {
                std::cout << "\nReleasing allocated playlist memory...\n";
                playlist.clear();
                std::cout << "Memory cleanup completed successfully.\n";
                std::cout << "Thank you for using the Music Playlist Management System. Goodbye!\n";
                running = false;
                break;
            }
            default:
                std::cout << "\n[Error] Invalid choice '" << choice << "'. Please select a valid option from 1 to 7.\n";
                break;
        }
    }

    return 0;
}
