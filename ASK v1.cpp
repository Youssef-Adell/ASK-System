#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//---Generic Functions---
int generateId(string filePath)
{
    ifstream fin(filePath);

    int generatedId;
    string line;

    if (fin.is_open())
    {
        while (getline(fin, line))
        {
            generatedId++;
        }
    }

    fin.close();
    return generatedId;
}

struct loginHandler
{
    int userId;
    string givenUserName;
    string givenFullName;
    string givenPassword;

    loginHandler()
    {
        userId = -1;
    }

    int menu()
    {
        int choice;

        system("cls");
        cout << "\nMenu:\n";
        cout << "\t1) Signup\n";
        cout << "\t2) login\n";
        cout << "\nEnter Choice [1:2]: ";
        cin >> choice;

        return choice;
    }
    int run()
    {
        bool isLogined = false;

        do
        {
            int choice = menu();

            switch (choice)
            {
            case 1:
                signup();
                break;
            case 2:
                isLogined = login();
                break;
            default:
                cout << "invalid choice, try again\n";
                break;
            }
        } while (!isLogined);

        return userId;
    }

    //---Useful Functions---
    bool isIdUsed(string givenUserName)
    {
        ifstream fin("users.txt");

        if (fin.is_open())
        {
            string line;
            string currentUserName;
            while (getline(fin, line))
            {
                istringstream userData(line);

                userData >> currentUserName; // to getride of id
                userData >> currentUserName; // to get currentUserName

                if (currentUserName == givenUserName)
                {
                    cout << "This Username is used before, try again\n";
                    fin.close();
                    return true;
                }
            }
        }

        fin.close();
        return false;
    }
    void saveUser(int id, string username, string fullName, string password)
    {
        ofstream fout("users.txt", ios::app);

        if (fout.is_open())
        {
            fout << id << " " << username << " " << fullName << " " << password << "\n";
        }

        fout.close();
    }
    int getUserId(string givenUserName, string givenPassword)
    {
        ifstream fin("users.txt");

        int currentId;
        string currentUserName;
        string currentPassword;
        string line;

        if (fin.is_open())
        {
            //---get line by line from file---
            while (getline(fin, line))
            {
                istringstream userData(line);
                //---extract data from the line---
                userData >> currentId;
                userData >> currentUserName;
                userData >> currentPassword; // to get ride of Full Name
                userData >> currentPassword;

                if (currentUserName == givenUserName && currentPassword == givenPassword)
                    return currentId;
            }
        }

        fin.close();
        return -1;
    }
    //---Primary Functions---
    void signup()
    {
        cout << "---Signup---\n";

        // read a username and ensure that it wasnt used before
        do
        {
            cout << "Username: ";
            cin >> givenUserName;
        } while (isIdUsed(givenUserName));

        cout << "Full Name: ";
        cin >> givenFullName;

        cout << "Password: ";
        cin >> givenPassword;
        system("cls");

        saveUser(generateId("users.txt") + 100, givenUserName, givenFullName, givenPassword);
    }
    bool login()
    {
        cout << "---Login---\n";

        cout << "Username: ";
        cin >> givenUserName;

        cout << "Password: ";
        cin >> givenPassword;
        system("cls");

        userId = getUserId(givenUserName, givenPassword);
        if (userId == -1)
        {
            cout << "invalid username or password\n";
            login();
        }
        return true;
    }
};

struct askSystem
{
    loginHandler login;
    int loginedUserId;

    askSystem()
    {
        loginedUserId = -1;
    }

    int menu()
    {
        int choice;
        cout << "\nMenu:\n";
        cout << "\t1) Print Question To me\n";
        cout << "\t2) Print Question from me\n";
        cout << "\t3) Answer Question\n";
        cout << "\t4) Delete Question\n";
        cout << "\t5) Ask Question\n";
        cout << "\t6) List System users\n";
        cout << "\t7) Feed\n";
        cout << "\t8) logout\n";
        cout << "\nEnter Choice[1:8]: ";
        cin >> choice;
        cout << "\n";
        return choice;
    }
    void run()
    {
        while (true)
        {
            // login if there is no user logined
            if (loginedUserId == -1)
                loginedUserId = login.run();

            int choice = menu();

            switch (choice)
            {

            case 1:
                questionsToMe();
                break;

            case 2:
                questionsFromMe();
                break;

            case 3:
                answerQuestion();
                break;

            case 5:
                askQuestion();
                break;

            case 6:
                listUsers();
                break;

            case 7:
                feed();
                break;

            case 8:
                loginedUserId = -1;
                break;

            default:
                cout << "invalid input, try again\n";
                break;
            }
        }
    }

    //---Helper Functions---
    void saveQuestion(int questionId, int senderId, int receiverId, string question)
    {
        ofstream fout("questions.txt", ios::app);

        if (fout.is_open())
        {
            fout << questionId << " " << senderId << " " << receiverId << " " << question << "\n";
            fout.close();
        }
    }
    void saveAnswer(int questionId, string answer)
    {
        ofstream fout("answers.txt", ios::app);

        if (fout.is_open())
        {
            fout << questionId << " " << answer << "\n";
            fout.close();
        }
    }

    bool isValidId(string filePath, int givenId)
    {
        ifstream fin(filePath);

        if (fin.is_open())
        {
            string line;
            int currentId;

            // get line by line from the file
            while (getline(fin, line))
            {
                // get id from the first of the line
                istringstream lineOfData(line);
                lineOfData >> currentId;

                if (currentId == givenId)
                {
                    fin.close();
                    return true;
                }
            }
        }
        cout << "Invalid Id, try again\n\n";
        return false;
    }
    bool isReceived(int questionId, int userId)
    {
        ifstream fin("questions.txt");

        if (fin.is_open())
        {
            string line;
            int currentQuestionId;
            int currentReceiverId;
            while (getline(fin, line))
            {
                istringstream questionData(line);
                questionData >> currentQuestionId;
                questionData >> currentReceiverId; // to get ride of sender id
                questionData >> currentReceiverId; // to get the reciver id to check it with given one

                // cout << "qId, rId: " << currentQuestionId << " " << currentReceiverId << "\n";
                if (currentQuestionId == questionId && currentReceiverId == userId)
                {
                    fin.close();
                    return true;
                }
            }
        }

        cout << "You didnt receive a question with this id \n";
        fin.close();
        return false;
    }

    void printQuestions(char mode = 'a', int userId = -1)
    {
        ifstream fin("questions.txt");

        if (fin.is_open())
        {
            string line;
            int questionId, senderId, receiverId;
            string question;

            // get question by question from file
            while (getline(fin, line))
            {
                istringstream questionData(line);
                // extract information of each question
                questionData >> questionId;
                questionData >> senderId;
                questionData >> receiverId;
                getline(questionData, question);

                // load questions sent or received by logined user into the map (depend on mode)
                if (userId == senderId && mode == 's')
                {
                    cout << "Question id (" << questionId << ") To user (" << receiverId << "): ";
                    cout << "Question: " << question << "\n";
                    printAnswer(questionId);
                }

                else if (userId == receiverId && mode == 'r')
                {
                    cout << "Question id (" << questionId << ") From user (" << senderId << "): \n";
                    cout << "\tQuestion: " << question << "\n";
                    printAnswer(questionId);
                }
                else if (mode == 'a' && userId == -1)
                {
                    cout << "Question id (" << questionId << ") from user (" << senderId << ") To user (" << receiverId << "): \n";
                    cout << "\tQuestion: " << question << "\n";
                    printAnswer(questionId);
                }
            }

            fin.close();
        }
    }
    void printAnswer(int questionId)
    {
        ifstream fin("answers.txt");

        if (fin.is_open())
        {
            string line;
            int answerId;
            string answer;

            while (getline(fin, line))
            {
                istringstream answerData(line);
                answerData >> answerId;
                getline(answerData, answer);

                if (answerId == questionId)
                {
                    cout << "\tAnswer\t: " << answer << "\n\n";
                    return;
                }
            }

            fin.close();
        }
        cout << "\tNot answerd yet\n\n";
    }
    //---Primary Functions---
    void questionsToMe()
    {
        printQuestions('r', loginedUserId);
    }
    void questionsFromMe()
    {
        printQuestions('s', loginedUserId);
    }
    void answerQuestion()
    {
        int questionId;
        string answer;
        string garbage;

        do
        {
            cout << "Enter question id: ";
            cin >> questionId;
        } while (!isReceived(questionId, loginedUserId));

        cout << "Enter Your Answer: ";
        getline(cin, garbage);
        getline(cin, answer);

        saveAnswer(questionId, answer);
    }
    void askQuestion()
    {
        int receiverId;
        string question;
        string garbage;

        do
        {
            cout << "Enter user id: ";
            cin >> receiverId;
        } while (!isValidId("users.txt", receiverId) || receiverId == loginedUserId);

        cout << "Enter Question text: ";
        getline(cin, garbage); // to get ride of garabage "\n" of last cin
        getline(cin, question);

        if (receiverId == -1)
            return;

        saveQuestion(generateId("questions.txt") + 500, loginedUserId, receiverId, question);
        cout << "Question Sent Successfully\n";
    }
    void listUsers()
    {
        ifstream fin("users.txt");

        if (fin.is_open())
        {
            string line;
            int userId;
            string fullName;
            while (getline(fin, line))
            {
                istringstream userData(line);

                userData >> userId;
                userData >> fullName; // to get ride of username
                userData >> fullName; // to get Full Name

                cout << "ID: " << userId << "\t\tFull Name: " << fullName << "\n";
            }

            fin.close();
        }
    }
    void feed()
    {
        cout << "\n---Feed---\n";
        printQuestions();
    }
};

int main()
{
    askSystem service;

    service.run();
}