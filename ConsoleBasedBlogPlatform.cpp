#include<iostream>
#include<filesystem>
#include<string>
#include<ctime>
#include<fstream>
#include<vector>
#include<map>
using std::cout;
using std::cin;
using std::string;
using std::endl;
class Post {
public:
    string postName;
    string Author;
    std::vector<string> postContent;
    time_t timestamp{};
    void writePost() {
        if (!std::filesystem::exists("Posts")) {
            std::filesystem::create_directory("Posts");
        }
        postName="";
        Author="";
        timestamp=std::time(nullptr);
        cout<<"Enter Post Title\n";
        getline(cin,postName);
        cout<<"Enter Author name\n";
        getline(cin,Author);
        cout<<"Enter Blog content and type \'END\' on a new line to finish writing\n";
        string Content;
        while (true) {
            std::getline(cin,Content);
            if (Content=="END") break;
            postContent.push_back(Content);
        }
    }
    void savePost() {
        string fileName="Posts/"+postName+"_"+std::to_string(timestamp)+".txt";
        std::ofstream file(fileName);
        file<<"Title: "+postName<<endl;
        file<<"Author: "+Author<<endl;
        file<<"Date: "<<std::ctime(&timestamp);
        for (auto line:postContent) {
            if (line=="END") break;
            file<<line<<endl;
        }
        file<<"\nEnd";
        file.close();
        cout<<"Post Saved as: "<<fileName<<endl;
    }
    void readPost() {
        std::vector<string> posts;
        for (const auto& textFile: std::filesystem::directory_iterator("Posts")) {
            if (textFile.path().extension()==".txt") {
                posts.push_back(textFile.path().string());
            }
        }
        if (posts.empty()) {
            cout<<"No files found";
            return;
        }
        cout<<"Select Blog Post to read: "<<endl;
        for (int i=0;i<posts.size();i++) {
            std::filesystem::path path(posts[i]);
            cout<<"Post :"<<(i+1)<<": "<<path.filename()<<endl;
        }
        cout<<"Enter a post number to read"<<endl;
        int choice;
        cin>>choice;
        cin.ignore();
        cout<<endl;
        if (choice>0 && choice<=posts.size()) {
            string text;
            std::ifstream readFile(posts[choice-1]);
            while (getline(readFile,text)) {
                cout<<text<<endl;
            }
            cout<<"-----x-----x-----x"<<endl;
            readFile.close();
        }
    }
    void editPost() {
        std::vector<string> posts;
        for (auto fileName: std::filesystem::directory_iterator("Posts")) {
            if (fileName.path().extension()==".txt"){
                posts.push_back(fileName.path().string());
            }
        }
        if (posts.empty()) {
            cout<<"No posts found";
            return;
        }
        cout<<"Please select a post to edit :"<<endl;
        for (int i=0;i<posts.size();i++) {
            std::filesystem::path path=posts[i];
            cout<<"Post :"<<(i+1)<<" "<<path.filename()<<endl;
        }
        int choice;
        cin>>choice;
        cin.ignore();
        std::ifstream readFile(posts[choice-1]);
        string text;
        string title,author,dateline;
        std::getline(readFile,title);
        cout<<title<<endl;
        std::getline(readFile,author);
        cout<<author<<endl;
        std::getline(readFile,dateline);
        cout<<dateline<<endl;
        std::vector<string> originalContent;
        while (std::getline(readFile,text) ){
            cout<<text<<endl;
            originalContent.push_back(text);
        }
        readFile.close();
        cout<<"Press 1 to add and 2 to remove content "<<endl;
        int editType;
        cin>>editType;
        cin.ignore();
        if (editType==1) {
            std::vector<string> newContent;
            cout<<"Please add content and write \'END\' on a new line to finish adding."<<endl;
            while (true) {
                string additionalContent;
                std::getline(cin,additionalContent);
                if (additionalContent=="END") break;
                newContent.push_back(additionalContent);
            }
            std::ofstream file(posts[choice-1]);
            file<<title<<endl;
            file<<author<<endl;
            file<<dateline<<endl;
            for (auto ogcontent:originalContent) {
                if (ogcontent=="End") break;
                file<<ogcontent<<endl;
            }
            for (auto updatedContent:newContent) {
                file<<updatedContent<<endl;
            }
            file<<"\nEnd";
            file.close();
        }
        if (editType==2) {
            cout<<"press to remove line numbers , press 0 to end: "<<endl;
            std::map<int,string> lines;
            for (int i=0;i<originalContent.size();i++) {
                if (originalContent[i]=="End") break;
                cout<<i+1<<" : "<<originalContent[i]<<endl;
                lines[i+1]=originalContent[i];
            }
            while (true) {
                int line;
                cin>>line;
                if (line==0) break;
                if (lines.empty()) break;
                lines.erase(line);
            }
            std::ofstream file(posts[choice-1]);
            file<<title<<endl;
            file<<author<<endl;
            file<<dateline<<endl;
            for (auto Line:lines) {
                if (Line.second=="End") break;
                file<<Line.second<<endl;
            }
            file<<"End";
        }
    };
    static void deletePost() {
        std::vector<string> posts;
        for (const auto& textFile:std::filesystem::directory_iterator("Posts")) {
            posts.push_back(textFile.path().string());
        }
        if (posts.empty()) {
            cout<<"No files found";
            return;
        }
        cout<<"Choose a file to delete"<<endl;
        for (int i=0;i<posts.size();i++) {
            std::filesystem::path path(posts[i]);
            cout<<"Post :"<<(i+1)<<": "<<path.filename()<<endl;
        }
        int choice;
        cin>>choice;
        cin.ignore();
        if (choice>0 && choice<=posts.size()){
            std::filesystem::path path(posts[choice-1]);
            string utf8path=path.u8string();
            const char *name=utf8path.c_str();
            int del=remove(name);
            if (del==0) {
                cout<<"File deleted successfully"<<endl;
            }
            else cout<<"Deletion failed"<<endl;
        }

    }

};
void blogMenu(){
    cout<<"Welcome to -The Writer's Blog- "<<endl;
    cout<<"Press 1: Read blog posts\n";
    cout<<"Press 2: Write blog post\n";
    cout<<"Press 3: Edit blog post\n";
    cout<<"Press 4: Delete blog post\n";
    cout<<"Press 0: Exit\n";
}
int main() {
    while (true) {
        blogMenu();
        Post blog;
        int command;
        cin>>command;
        cin.ignore();
        if (command==1) {
            blog.readPost();
        }
        if (command==2) {
            blog.writePost();
            blog.savePost();
        }
        if (command==3) {
            blog.editPost();
        }
        if (command==4) {
            Post::deletePost();
        }
        if (command==0) {
            exit(0);
        }
    }
}
