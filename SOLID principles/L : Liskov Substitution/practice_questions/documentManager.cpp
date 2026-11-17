/*
Problem 1: Refactoring a Broken Hierarchy — Document Editor

The Scenario : You are building a document management app. A developer wrote the following hierarchy:

Your Task
Q1. Connection to OCP (Quick Recall): If a client function "autoSaveAll(vector<Document*>& docs)" wants to avoid crashing on PdfReadOnlyDocument without changing the class hierarchy, what bad check would the developer be forced to write inside autoSaveAll, and how does that break OCP?

Ans: The developer will have to implement a 'if' condition check to see what is the type of 'Document' in the vector docs, before calling the save function on that doc.
-> in C++, checking a pointer's concrete subtype at runtime requires RTTI (dynamic_cast<PdfReadOnlyDocument*>(doc) == nullptr).
-> The developer would be forced to add a runtime type-check (e.g., if (dynamic_cast<PdfReadOnlyDocument*>(doc) == nullptr)) inside autoSaveAll() before calling doc->save(). This directly breaks OCP because autoSaveAll() is no longer closed for modification—every time a new read-only document subtype (like SignedContractDocument or ArchivedDocument) is added in the future, the developer must modify autoSaveAll() to add another dynamic_cast check. (Violating LSP in the hierarchy forced the client code to violate OCP.)


2. Refactor for LSP: Redesign the classes in C++ so that read-only documents and editable documents can be used polymorphically without dummy/throwing save() overrides, and write a main() driver demonstrating both.

Ans. One way to do it as I can think is as follow:
    1. Make a Document class, which is read-only by default, because Read is a common functionality between them both. 
    - For EditanleDocument we make a derived class that is for Editable document. This class inherits from read-only type Document, and adds Edit and Save functionalities.  
    - All Read-Only derived classes can inherit from Document as it's read only by default. 
    - Editable document will provide functionality to edit and save changes so, all editable documents can extend this Editable class.

                   ┌──────────────────────────┐
                   │         Document         │
                   │ (Read-only by default)   │
                   │ + open()                 │
                   └─────────────┬────────────┘
                                 │
            ┌────────────────────┴────────────────────┐
            ▼                                         ▼
┌───────────────────────┐                 ┌───────────────────────┐
│  PdfReadOnlyDocument  │                 │   EditableDocument    │
│  (Inherits open())    │                 │   + save(newContent)  │
└───────────────────────┘                 └───────────┬───────────┘
                                                      │
                                          ┌───────────┴───────────┐
                                          ▼                       ▼
                                ┌───────────────────┐   ┌───────────────────┐
                                │   WordDocument    │   │   TextDocument    │
                                └───────────────────┘   └───────────────────┘
    
    2. Second way I can think of is by utilizing Composition. 
    - We can have a ReadableDoc() and a EditableDoc() and then the specific classes can inherit the properties of these two classes to build their own functionality.
    - i.e. PdfDocument can use ReadableDo, and WordDocument can use Editable and Readable both.
*/

#include <iostream>
#include <string>
using namespace std;

// This is the base class and by default it's read only
class Document {
protected:
    std::string content;
public:
    Document(const std::string& text) : content(text) {}
    virtual ~Document() = default;

    virtual void open() const {
        std::cout << "Opening document. Content: " << content << endl;
    }
};

// Editable Document is a classs that makes a document editable by providing that functionality
class EditableDocument : public Document {
public:
    EditableDocument(const string& text) : Document(text){}

    virtual void writeAndSave(const std::string& newContent) {
        content = content + newContent;
        std::cout << "Document Overwittten and Saved with new content." << endl;
    }
};

class WordDocument : public EditableDocument {
public:
    WordDocument(const string& text) : EditableDocument(text){}

};

class PdfReadOnlyDocument : public Document {
public:
    PdfReadOnlyDocument(const std::string& text) : Document(text) {}
};


// ----------------------------------------------------

void openDocument(const Document& doc){
        doc.open();
    }

void writeToDocumentAndSave(EditableDocument& doc, const string& text) {
    doc.writeAndSave(text);
}



int main(){

    PdfReadOnlyDocument pdf("This is a read-only file.");
    
    openDocument(pdf);
    // writeToDocumentAndSave(pdf, "add this to pdf"); // won't work


    WordDocument wordDoc("This is an Editable document.");

    openDocument(wordDoc);
    writeToDocumentAndSave(wordDoc, " This is an addition to word doc.");
    cout << "Word Document after Editing and Saving : " << endl;
    openDocument(wordDoc); // will display new content

    return 0;
}



/*
TAKE AWAYS FROM THIS EXERCISE:

1. LLD & Design Takeaways (The "Why")
    1. Base Classes Should Only Hold Universal Capabilities:
        - If even one valid subtype cannot support a method (like PdfReadOnlyDocument not supporting save()), that method does not belong in the base class.
        
        - Put the lowest common denominator like open() in the base class (Document), and push specialized capabilities like writeAndSave() down into an intermediate class (EditableDocument).

    2. Turn Runtime Crashes into Compile-Time Errors:
        - Before (Bad LSP): Passing pdf to save() compiled fine and crashed at runtime with an exception.
        - After (Good LSP): writeToDocumentAndSave(pdf, ...) (line 103) is rejected by the compiler immediately. The type system now protects you.


    3. ** Violating LSP Forces Client Code to Violate OCP: ** IMP
        - Whenever a child class throws "Not Supported" in an inherited method, client loops are forced to add if (dynamic_cast<Child*>(obj)) checks—meaning client code must be modified every time a new restricted subtype is added.

2. C++ Mechanics Takeaways (The "How")
    1. protected Members Are Already Inside Every Child Object:
        - Because std::string content; is protected in Document (line 53), EditableDocument and WordDocument already own that variable.
        - Rule of thumb: Never redeclare a parent's member variable in a child class—doing so creates a second, hidden copy (variable shadowing).

    2. Constructor Chaining (Child(args) : Parent(args) {}):
        - Constructors are not inherited automatically, so each child constructor passes its initial data up to its immediate parent in the initializer list (line 66 and line 76): WordDocument → EditableDocument → Document.

    3. 'const' Correctness Tells the Caller's Intent:
        - openDocument(const Document& doc) (line 88) takes a const reference because it only reads (open() const).
        - writeToDocumentAndSave(EditableDocument& doc, ...) (line 92) takes a non-const reference because it mutates doc.
*/