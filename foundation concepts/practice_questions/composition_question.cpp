/*
    Q6 — Composition (has-a)

    Build: Song (title, seconds) and Playlist.

    Playlist has songs. Playlist is not a Song. Do not inherit.

    Playlist:
    - void add(const Song&)
    - int totalSeconds() const
    - void list() const — print each title
    
    TEST:
    main: add three songs, print the list, print total duration.

    Done when: you can say this is has-a, not is-a, and why inheritance would be the wrong model.
*/

#include <iostream>
#include <vector>
using namespace std;

class Song{
    
    string title;
    int seconds;

public:
    Song(string t, int s) : title(t), seconds(s) {}

    string getTitle() const{
        return title;
    }

    int getSeconds() const{
        return seconds;
    }

    ~Song(){}
};

// Playlist has-a Song.

class Playlist{
    vector<Song> songsInList; // storing the songs in playlist - Shows "has-a" relation between playlist and song
    int totalDuration; 

public:
    Playlist(){
        songsInList = {};
        totalDuration = 0;
    }

    void add(const Song& s){
        songsInList.push_back(s);
        totalDuration += s.getSeconds(); //increment the total seconds as the songs get added.
    }

    int totalSeconds() const{
        return totalDuration;
    }

    void list() const{
        for(const Song song : songsInList){
            cout << song.getTitle() << endl;
        }
    }

    ~Playlist(){
    }
};


int main(){

    Song s1("Interstellar theme", 250);
    Song s2("Inception Theme", 200);
    Song s3("Oppenhimer Theme", 300);

    Playlist themes;
    themes.add(s1);
    themes.add(s2);
    themes.add(s3);

    themes.list();
    cout << themes.totalSeconds() << endl;

    return 0;
}