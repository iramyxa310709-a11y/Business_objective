#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

string readLine(string prompt)
{
    cout << prompt;
    string s;
    getline(cin, s);
    if (!s.empty() && s[s.size() - 1] == '\r') s.erase(s.size() - 1);
    return s;
}

int readInt(string prompt)
{
    string s = readLine(prompt);
    if (!cin) return 0;
    try
    {
        return stoi(s);
    }
    catch (...)
    {
        return -1;
    }
}

class Item
{
private:
    string name;
public:
    Item(string n = "") : name(n) {}
    virtual ~Item() {}

    string getName() const { return name; }
    void setName(string n) { name = n; }

    virtual void print() const { cout << name << endl; }
};

class Question
{
public:
    string text;
    vector<string> answers;
    int correct;

    Question() : correct(0) {}

    void Save(ofstream& out) const
    {
        out << text << endl;
        out << answers.size() << endl;
        for (size_t i = 0; i < answers.size(); i++)
            out << answers[i] << endl;
        out << correct << endl;
    }

    void Load(ifstream& in)
    {
        int n = 0;
        getline(in, text);
        in >> n;
        in.ignore();
        answers.clear();
        for (int i = 0; i < n; i++)
        {
            string a;
            getline(in, a);
            answers.push_back(a);
        }
        in >> correct;
        in.ignore();
    }
};

class Test : public Item
{
public:
    vector<Question> questions;

    Test(string n = "") : Item(n) {}

    void print() const
    {
        cout << "Тест: " << getName() << " (питань: " << questions.size() << ")" << endl;
    }

    void Save(ofstream& out) const
    {
        out << getName() << endl;
        out << questions.size() << endl;
        for (size_t i = 0; i < questions.size(); i++)
            questions[i].Save(out);
    }

    void Load(ifstream& in)
    {
        string n;
        int count = 0;
        getline(in, n);
        setName(n);
        in >> count;
        in.ignore();
        questions.clear();
        for (int i = 0; i < count; i++)
        {
            Question q;
            q.Load(in);
            questions.push_back(q);
        }
    }
};

class Category : public Item
{
public:
    vector<Test> tests;

    Category(string n = "") : Item(n) {}

    void print() const
    {
        cout << "Розділ: " << getName() << endl;
        for (size_t i = 0; i < tests.size(); i++)
        {
            cout << "   " << i + 1 << ". ";
            tests[i].print();
        }
    }

    void Save(ofstream& out) const
    {
        out << getName() << endl;
        out << tests.size() << endl;
        for (size_t i = 0; i < tests.size(); i++)
            tests[i].Save(out);
    }

    void Load(ifstream& in)
    {
        string n;
        int count = 0;
        getline(in, n);
        setName(n);
        in >> count;
        in.ignore();
        tests.clear();
        for (int i = 0; i < count; i++)
        {
            Test t;
            t.Load(in);
            tests.push_back(t);
        }
    }
};

struct User
{
    string login, pass;
    string fio, address, phone;
};

struct Result
{
    string login;
    string category, test;
    int correct, total;
};

struct Progress
{
    string login;
    string category, test;
    int nextQuestion, correct;
};

vector<Category> categories;
vector<User> users;
vector<Result> results;
vector<Progress> progresses;
string adminLogin, adminPass;

void saveTests(string fname)
{
    ofstream out(fname.c_str());
    out << categories.size() << endl;
    for (size_t i = 0; i < categories.size(); i++)
        categories[i].Save(out);
}

void loadTests(string fname, bool silent)
{
    ifstream in(fname.c_str());
    if (!in.is_open())
    {
        if (!silent) cout << "Файл не знайдено!" << endl;
        return;
    }
    int n = 0;
    in >> n;
    in.ignore();
    for (int i = 0; i < n; i++)
    {
        Category c;
        c.Load(in);
        categories.push_back(c);
    }
}

void saveUsers()
{
    ofstream out("users.txt");
    out << users.size() << endl;
    for (size_t i = 0; i < users.size(); i++)
        out << users[i].login << endl << users[i].pass << endl << users[i].fio << endl
            << users[i].address << endl << users[i].phone << endl;
}

void loadUsers()
{
    ifstream in("users.txt");
    if (!in.is_open()) return;
    int n = 0;
    in >> n;
    in.ignore();
    for (int i = 0; i < n; i++)
    {
        User u;
        getline(in, u.login);
        getline(in, u.pass);
        getline(in, u.fio);
        getline(in, u.address);
        getline(in, u.phone);
        users.push_back(u);
    }
}

void saveResults()
{
    ofstream out("results.txt");
    out << results.size() << endl;
    for (size_t i = 0; i < results.size(); i++)
        out << results[i].login << endl << results[i].category << endl << results[i].test << endl
            << results[i].correct << endl << results[i].total << endl;
}

void loadResults()
{
    ifstream in("results.txt");
    if (!in.is_open()) return;
    int n = 0;
    in >> n;
    in.ignore();
    for (int i = 0; i < n; i++)
    {
        Result r;
        getline(in, r.login);
        getline(in, r.category);
        getline(in, r.test);
        in >> r.correct >> r.total;
        in.ignore();
        results.push_back(r);
    }
}

void saveProgress()
{
    ofstream out("progress.txt");
    out << progresses.size() << endl;
    for (size_t i = 0; i < progresses.size(); i++)
        out << progresses[i].login << endl << progresses[i].category << endl << progresses[i].test << endl
            << progresses[i].nextQuestion << endl << progresses[i].correct << endl;
}

void loadProgress()
{
    ifstream in("progress.txt");
    if (!in.is_open()) return;
    int n = 0;
    in >> n;
    in.ignore();
    for (int i = 0; i < n; i++)
    {
        Progress p;
        getline(in, p.login);
        getline(in, p.category);
        getline(in, p.test);
        in >> p.nextQuestion >> p.correct;
        in.ignore();
        progresses.push_back(p);
    }
}

void saveAdmin()
{
    ofstream out("admin.txt");
    out << adminLogin << endl << adminPass << endl;
}

void loadAdmin()
{
    ifstream in("admin.txt");
    getline(in, adminLogin);
    getline(in, adminPass);
}

bool loginExists(string login)
{
    if (login == adminLogin) return true;
    for (size_t i = 0; i < users.size(); i++)
        if (users[i].login == login) return true;
    return false;
}

int calcGrade(int correct, int total)
{
    if (total == 0) return 0;
    int g = (correct * 12 + total / 2) / total;
    if (g < 1) g = 1;
    return g;
}

int calcPercent(int correct, int total)
{
    if (total == 0) return 0;
    return correct * 100 / total;
}

string resultLine(const Result& r)
{
    return r.login + " | " + r.category + " | " + r.test + " | " +
           to_string(r.correct) + "/" + to_string(r.total) + " | " +
           to_string(calcPercent(r.correct, r.total)) + "% | оцінка: " +
           to_string(calcGrade(r.correct, r.total));
}

void showUsers()
{
    for (size_t i = 0; i < users.size(); i++)
        cout << i + 1 << ". " << users[i].login << " | " << users[i].fio << " | "
             << users[i].address << " | " << users[i].phone << endl;
}

void showCategories()
{
    for (size_t i = 0; i < categories.size(); i++)
    {
        cout << i + 1 << ". ";
        categories[i].print();
    }
}

void takeTest(string login)
{
    if (categories.empty())
    {
        cout << "Тестів ще немає." << endl;
        return;
    }
    cout << "Розділи:" << endl;
    for (size_t i = 0; i < categories.size(); i++)
        cout << i + 1 << ". " << categories[i].getName() << endl;
    int c = readInt("Оберіть розділ: ");
    if (c < 1 || c > (int)categories.size()) { cout << "Невірний вибір" << endl; return; }
    Category& cat = categories[c - 1];
    if (cat.tests.empty()) { cout << "У розділі немає тестів." << endl; return; }
    cat.print();
    int t = readInt("Оберіть тест: ");
    if (t < 1 || t > (int)cat.tests.size()) { cout << "Невірний вибір" << endl; return; }
    Test& test = cat.tests[t - 1];
    if (test.questions.empty()) { cout << "У тесті немає питань." << endl; return; }

    int start = 0, correct = 0;
    for (size_t i = 0; i < progresses.size(); i++)
    {
        if (progresses[i].login == login && progresses[i].category == cat.getName() &&
            progresses[i].test == test.getName())
        {
            int ans = readInt("Є незавершений тест. 1 - продовжити, 0 - почати спочатку: ");
            if (ans == 1)
            {
                start = progresses[i].nextQuestion;
                correct = progresses[i].correct;
            }
            progresses.erase(progresses.begin() + i);
            saveProgress();
            break;
        }
    }

    int total = (int)test.questions.size();
    for (int i = start; i < total; i++)
    {
        Question& q = test.questions[i];
        cout << endl << "Питання " << i + 1 << "/" << total << ": " << q.text << endl;
        for (size_t a = 0; a < q.answers.size(); a++)
            cout << "  " << a + 1 << ") " << q.answers[a] << endl;
        int ans = -1;
        while (ans < 0 || ans > (int)q.answers.size())
            ans = readInt("Ваша відповідь (0 - перервати тест): ");
        if (ans == 0)
        {
            Progress p;
            p.login = login;
            p.category = cat.getName();
            p.test = test.getName();
            p.nextQuestion = i;
            p.correct = correct;
            progresses.push_back(p);
            saveProgress();
            cout << "Тест перервано, можна продовжити пізніше." << endl;
            return;
        }
        if (ans - 1 == q.correct) correct++;
    }

    Result r;
    r.login = login;
    r.category = cat.getName();
    r.test = test.getName();
    r.correct = correct;
    r.total = total;
    results.push_back(r);
    saveResults();
    cout << endl << "Тест завершено!" << endl;
    cout << "Правильних відповідей: " << correct << " з " << total << endl;
    cout << "Відсоток: " << calcPercent(correct, total) << "%" << endl;
    cout << "Оцінка (12-бальна): " << calcGrade(correct, total) << endl;
}

void PrintGuestMenu()
{
    cout << endl << "=== Меню гостя ===" << endl;
    cout << "1. Мої результати" << endl;
    cout << "2. Здати тест" << endl;
    cout << "0. Вийти" << endl;
}

void ShowMyResults(string login)
{
    bool any = false;
    for (size_t i = 0; i < results.size(); i++)
    {
        if (results[i].login == login)
        {
            cout << resultLine(results[i]) << endl;
            any = true;
        }
    }
    if (!any) cout << "Результатів ще немає." << endl;
}

void guestMenu(string login)
{
    int choice = -1;
    while (choice != 0)
    {
        PrintGuestMenu();
        choice = readInt("> ");

        if (choice == 1)
            ShowMyResults(login);
        else if (choice == 2)
            takeTest(login);
        else if (choice == 0)
            cout << "Вихід." << endl;
        else
            cout << "Невірний вибір." << endl;
    }
}

void registerUser()
{
    User u;
    u.login = readLine("Логін: ");
    if (u.login.empty() || loginExists(u.login))
    {
        cout << "Такий логін вже є або він порожній!" << endl;
        return;
    }
    u.pass = readLine("Пароль: ");
    u.fio = readLine("ПІБ: ");
    u.address = readLine("Домашня адреса: ");
    u.phone = readLine("Телефон: ");
    users.push_back(u);
    saveUsers();
    cout << "Реєстрація успішна!" << endl;
}

void PrintUserMenu()
{
    cout << endl << "--- Керування користувачами ---" << endl;
    cout << "1. Список" << endl;
    cout << "2. Створити" << endl;
    cout << "3. Видалити" << endl;
    cout << "4. Змінити" << endl;
    cout << "0. Назад" << endl;
}

void DeleteUser()
{
    showUsers();
    int n = readInt("Номер для видалення: ");
    if (n >= 1 && n <= (int)users.size())
    {
        users.erase(users.begin() + (n - 1));
        saveUsers();
        cout << "Видалено." << endl;
    }
}

void EditUser()
{
    showUsers();
    int n = readInt("Номер для зміни: ");
    if (n >= 1 && n <= (int)users.size())
    {
        User& u = users[n - 1];
        cout << "(Enter - залишити без змін)" << endl;
        string s = readLine("Новий логін: ");
        if (!s.empty())
        {
            if (loginExists(s)) cout << "Такий логін вже є, логін не змінено." << endl;
            else u.login = s;
        }
        s = readLine("Новий пароль: ");
        if (!s.empty()) u.pass = s;
        s = readLine("Новий ПІБ: ");
        if (!s.empty()) u.fio = s;
        s = readLine("Нова адреса: ");
        if (!s.empty()) u.address = s;
        s = readLine("Новий телефон: ");
        if (!s.empty()) u.phone = s;
        saveUsers();
        cout << "Збережено." << endl;
    }
}

void manageUsers()
{
    int choice = -1;
    while (choice != 0)
    {
        PrintUserMenu();
        choice = readInt("> ");

        if (choice == 1)
            showUsers();
        else if (choice == 2)
            registerUser();
        else if (choice == 3)
            DeleteUser();
        else if (choice == 4)
            EditUser();
        else if (choice == 0)
            cout << "Назад." << endl;
        else
            cout << "Невірний вибір." << endl;
    }
}

void statistics()
{
    cout << endl << "--- Статистика ---" << endl;
    cout << "1. По розділу (Enter - всі розділи)" << endl;
    cout << "2. По тесту" << endl;
    cout << "3. По користувачу" << endl;
    int ch = readInt("> ");
    string cat, test, login;
    if (ch == 1) cat = readLine("Назва розділу: ");
    else if (ch == 2) { cat = readLine("Назва розділу: "); test = readLine("Назва тесту: "); }
    else if (ch == 3) login = readLine("Логін користувача: ");
    else return;

    string text;
    for (size_t i = 0; i < results.size(); i++)
    {
        Result& r = results[i];
        bool ok = true;
        if (ch == 1 && !cat.empty() && r.category != cat) ok = false;
        if (ch == 2 && (r.category != cat || r.test != test)) ok = false;
        if (ch == 3 && r.login != login) ok = false;
        if (ok) text += resultLine(r) + "\n";
    }
    if (text.empty()) text = "Результатів не знайдено.\n";
    cout << text;
    if (readInt("Записати у файл? 1 - так, 0 - ні: ") == 1)
    {
        string fname = readLine("Ім'я файлу: ");
        ofstream f(fname.c_str());
        f << text;
        cout << "Записано." << endl;
    }
}

void addQuestion()
{
    showCategories();
    int c = readInt("Номер розділу: ");
    if (c < 1 || c > (int)categories.size()) return;
    Category& cat = categories[c - 1];
    cat.print();
    int t = readInt("Номер тесту: ");
    if (t < 1 || t > (int)cat.tests.size()) return;
    Question q;
    q.text = readLine("Текст питання: ");
    int n = readInt("Кількість відповідей: ");
    for (int i = 0; i < n; i++)
    {
        q.answers.push_back(readLine("Відповідь " + to_string(i + 1) + ": "));
    }
    int right = readInt("Номер правильної відповіді: ");
    if (right < 1 || right > n)
    {
        cout << "Невірний номер, питання не додано." << endl;
        return;
    }
    q.correct = right - 1;
    cat.tests[t - 1].questions.push_back(q);
    saveTests("tests.txt");
    cout << "Питання додано." << endl;
}

void PrintTestMenu()
{
    cout << endl << "--- Керування тестами ---" << endl;
    cout << "1. Показати розділи і тести" << endl;
    cout << "2. Додати розділ" << endl;
    cout << "3. Додати тест" << endl;
    cout << "4. Додати питання" << endl;
    cout << "5. Імпорт з файлу" << endl;
    cout << "6. Експорт у файл" << endl;
    cout << "0. Назад" << endl;
}

void AddCategory()
{
    string name = readLine("Назва розділу: ");
    if (!name.empty())
    {
        categories.push_back(Category(name));
        saveTests("tests.txt");
    }
}

void AddTest()
{
    showCategories();
    int c = readInt("Номер розділу: ");
    if (c >= 1 && c <= (int)categories.size())
    {
        string name = readLine("Назва тесту: ");
        if (!name.empty())
        {
            categories[c - 1].tests.push_back(Test(name));
            saveTests("tests.txt");
        }
    }
}

void ImportTests()
{
    string fname = readLine("Ім'я файлу: ");
    loadTests(fname, false);
    saveTests("tests.txt");
}

void ExportTests()
{
    string fname = readLine("Ім'я файлу: ");
    saveTests(fname);
    cout << "Експортовано." << endl;
}

void manageTests()
{
    int choice = -1;
    while (choice != 0)
    {
        PrintTestMenu();
        choice = readInt("> ");

        if (choice == 1)
            showCategories();
        else if (choice == 2)
            AddCategory();
        else if (choice == 3)
            AddTest();
        else if (choice == 4)
            addQuestion();
        else if (choice == 5)
            ImportTests();
        else if (choice == 6)
            ExportTests();
        else if (choice == 0)
            cout << "Назад." << endl;
        else
            cout << "Невірний вибір." << endl;
    }
}

void PrintAdminMenu()
{
    cout << endl << "=== Меню адміністратора ===" << endl;
    cout << "1. Керування користувачами" << endl;
    cout << "2. Статистика" << endl;
    cout << "3. Керування тестами" << endl;
    cout << "4. Змінити логін і пароль адміна" << endl;
    cout << "0. Вийти" << endl;
}

void ChangeAdmin()
{
    string l = readLine("Новий логін: ");
    string p = readLine("Новий пароль: ");
    if (l.empty() || p.empty())
    {
        cout << "Не можна залишати порожнім." << endl;
    }
    else if (l != adminLogin && loginExists(l))
    {
        cout << "Такий логін вже зайнятий." << endl;
    }
    else
    {
        adminLogin = l;
        adminPass = p;
        saveAdmin();
        cout << "Змінено." << endl;
    }
}

void adminMenu()
{
    int choice = -1;
    while (choice != 0)
    {
        PrintAdminMenu();
        choice = readInt("> ");

        if (choice == 1)
            manageUsers();
        else if (choice == 2)
            statistics();
        else if (choice == 3)
            manageTests();
        else if (choice == 4)
            ChangeAdmin();
        else if (choice == 0)
            cout << "Вихід." << endl;
        else
            cout << "Невірний вибір." << endl;
    }
}

void PrintMenu()
{
    cout << endl << "=== Система тестування ===" << endl;
    cout << "1. Вхід" << endl;
    cout << "2. Реєстрація" << endl;
    cout << "0. Вихід" << endl;
}

void SetupAdmin()
{
    cout << "Перший запуск. Створіть адміністратора." << endl;
    while (adminLogin.empty()) adminLogin = readLine("Логін адміна: ");
    while (adminPass.empty()) adminPass = readLine("Пароль адміна: ");
    saveAdmin();
}

void Login()
{
    string l = readLine("Логін: ");
    string p = readLine("Пароль: ");
    if (l == adminLogin && p == adminPass)
    {
        adminMenu();
        return;
    }
    for (size_t i = 0; i < users.size(); i++)
    {
        if (users[i].login == l && users[i].pass == p)
        {
            guestMenu(l);
            return;
        }
    }
    cout << "Невірний логін або пароль!" << endl;
}

int main()
{
    loadTests("tests.txt", true);
    loadUsers();
    loadResults();
    loadProgress();
    loadAdmin();

    if (adminLogin.empty())
        SetupAdmin();

    int choice = -1;
    while (choice != 0)
    {
        PrintMenu();
        choice = readInt("> ");

        if (choice == 1)
            Login();
        else if (choice == 2)
            registerUser();
        else if (choice == 0)
            cout << "Вихід." << endl;
        else
            cout << "Невірний вибір." << endl;
    }
    return 0;
}