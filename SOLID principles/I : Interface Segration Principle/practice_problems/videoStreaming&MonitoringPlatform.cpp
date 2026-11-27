/*

Problem 2 : Client-Side ISP — Video Streaming & Moderation Platform (Medium)

The Scenario :
Sometimes one single backend server class genuinely supports many operations, so developers think a fat interface is fine. Look at this 'IContentPlatform' interface implemented by 'MainVideoServer'

```     class IContentPlatform {
        public:
            virtual ~IContentPlatform() = default;

            // 1. Viewer operations
            virtual void streamVideo(const std::string& videoId) const = 0;

            // 2. Creator operations
            virtual void uploadVideo(const std::string& title) = 0;

            // 3. Admin / Moderator operations
            virtual void banVideo(const std::string& videoId) = 0;
            virtual void purgeDatabaseCache() = 0;
```     };


- Currently, three separate UI modules (ViewerApp, CreatorStudio, and AdminConsole) all accept 'IContentPlatform&' 
in their constructors. Later, the infrastructure team also wants to add a 'ReadOnlyEdgeCDNServer' that caches 
videos close to users and only supports streamVideo().

Your Task :
    1. Analyze the Flaws (Write in comments):
    - Client-Side Problem: Even before 'ReadOnlyEdgeCDNServer' existed, why was passing 'IContentPlatform&' 
    to ViewerApp a violation of ISP and an architectural risk?
    - Implementer-Side Problem: What happens when 'ReadOnlyEdgeCDNServer' tries to implement 'IContentPlatform'?


    2. Refactor & Implement in C++:
    - Split 'IContentPlatform' into three role-specific interfaces: 'IContentViewer', 'IContentCreator', 
        and 'IContentModerator'.

    - Implement 'MainVideoServer' (implements all 3 interfaces) and 'ReadOnlyEdgeCDNServer' (implements only 'IContentViewer').

    - Create 'ViewerApp', 'CreatorStudio', and 'AdminConsole' (each following SRP and taking only the exact interface role it needs).

    - In main(), show 'ViewerApp' working seamlessly with both 'MainVideoServer' and 'ReadOnlyEdgeCDNServer'.

*/


/* 
    Q1. PROBLEMS / FLAWS in IContentPlatform :

    1. Client-Side Problem (Passing 'IContentPlatform&' to 'ViewerApp'):
    -> If 'IContentPlatform&' is passed to 'ViewerApp', a viewer should only be able to stream videos. However, this 
    fat interface allows 'ViewerApp' to call 'uploadVideo()', and even crazier, gives it access to 'banVideo()' and 
    'purgeDatabaseCache()', which should only be accessible to admins with permission! (Violation of Principle of 
    Least Privilege / API encapsulation)
        
        -> Because 'MainVideoServer' genuinely implements all 4 methods (it doesn't throw exceptions!), it is even 
        more dangerous: if 'ViewerApp' accidentally calls 'platform.purgeDatabaseCache()', it won't throw an error—it 
        will actually delete the cache!
        
        -> Unnecessary Coupling & Recompilation : Because 'ViewerApp' depends on 'IContentPlatform', if an admin 
        method signature changes (e.g., 'banVideo(videoId)' becomes 'banVideo(videoId, reason)'), 'ViewerApp' is 
        forced to recompile and redeploy even though it never uses admin methods.
        

    2. Implementer-Side Problem ('ReadOnlyEdgeCDNServer' implementing 'IContentPlatform'):
        -> 'IContentPlatform' has too many methods bundled together, which forces any class implementing this interface to define methods it doesn't even support.
        
        -> When 'ReadOnlyEdgeCDNServer' tries to implement 'IContentPlatform', it only supports 'streamVideo()', yet 
        it is forced to implement 'uploadVideo()', 'banVideo()', and 'purgeDatabaseCache()' as dummy no-ops or by 
        raising exceptions for functions it won't be using.
        
        -> That directly breaks the LSP principle (any client holding an 'IContentPlatform&' and expecting those 
        methods to work will crash at runtime if handed a 'ReadOnlyEdgeCDNServer').
*/



// REFACTORING CODE

#include <iostream>
using namespace std;

// ----------INTERFACES-------------

// Viewer operations
class IContentViewer {
public:
    virtual ~IContentViewer() = default;
    virtual void streamVideo(const std::string& videoId) const = 0;
};

// Creator operations
class IContentCreator {
public:
    virtual ~IContentCreator() = default;
    virtual void uploadVideo(const std::string& title) = 0;
};

// Admin / Moderator operations
class IContentModerator {
public:
    virtual ~IContentModerator() = default;
    virtual void banVideo(const std::string& videoId) = 0;
    virtual void purgeDatabaseCache() = 0;
};


// ------------------SERVERS-------------------

// 1. Main Server - Implements all 3 interface
class MainVideoServer : public IContentCreator, public IContentViewer, public IContentModerator {

public: 
    void streamVideo(const string& videoId)const override {
        // streams video logic
        cout << "Streaming video from Main Server : " << videoId << endl;
    }

    void uploadVideo(const std::string& title) override{
        // upload video logic
        cout << "[ " << title << " ] upoaded." << endl;
    }

    void banVideo(const std::string& videoId) override {
        // ban video logic
        cout << "[ " << videoId << " ] banned." << endl;
    }

    void purgeDatabaseCache() override {
        // logic
        cout << "Cleaned DB Cache" << endl;
    }
};


// 2. READ ONLY Edge CDN Server - Implements only 'IContentViewer'
class ReadOnlyEdgeCDNServer : public IContentViewer {
public:
    void streamVideo(const std::string& videoId) const override {
        // streams video logic
        cout << "Streaming video from Edge CDN : " << videoId << endl;
    }
};


// ------------ UI Modules---------------
// 'ViewerApp', 'CreatorStudio', and 'AdminConsole'

// As per the instructions in Question these are 3 separate UI modules (ViewerApp, CreatorStudio, and AdminConsole) all accept the interface in their constructors.

// That means 'ViewerApp', 'CreatorStudio', and 'AdminConsole' are CLIENT CLASSES that use (hold a reference to) a server interface ("has-a" composition, not "is-a" inheritance).

class ViewerApp {
    const IContentViewer& videoStreamer;
public:
    ViewerApp(const IContentViewer& contentViewer) : videoStreamer(contentViewer) {}

    void watch(const string& videoId) const {
        videoStreamer.streamVideo(videoId);
    }
};

class CreatorStudio {
    IContentCreator& creator; 
public:
    CreatorStudio(IContentCreator& videoServer) : creator(videoServer) {}
    
    void upload(const std::string& vidTitle) {
        creator.uploadVideo(vidTitle);
    }
};

class AdminConsole {
    IContentModerator& moderator;
public:
    AdminConsole(IContentModerator& contentModerator) : moderator(contentModerator) {}

    void banVideo(const string& vidId){
        moderator.banVideo(vidId);
    }

    void clearDBCache(){
        moderator.purgeDatabaseCache();
    }
};

// In main(), show 'ViewerApp' working seamlessly with both 'MainVideoServer' and 'ReadOnlyEdgeCDNServer'.

int main(){

    MainVideoServer mainServer;
    ReadOnlyEdgeCDNServer edgeCdnServer;

    // 1. ViewerApp connected to MainVideoServer (only sees IContentViewer methods!)
    ViewerApp viewerOnMain(mainServer);
    viewerOnMain.watch("Interstellar");

     // 2. ViewerApp connected to ReadOnlyEdgeCDNServer (works seamlessly!)
    ViewerApp viewerOnCDN(edgeCdnServer);
    viewerOnCDN.watch("Inception");

    // 3. CreatorStudio and AdminConsole connected to MainVideoServer
    CreatorStudio studio(mainServer);
    studio.upload("Understanding Riemann Hypothesys");

    AdminConsole admin(mainServer);
    admin.banVideo("GTA 6 Leaked clips");
    admin.clearDBCache();

    return 0;
}