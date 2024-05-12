#include <iostream>
#include <string>
#include "sqlite3/sqlite3.h"
#include <regex>
#include <vector>
using namespace std;
void menu();
void rejestr();
void login();
void tworzenie_bazy();
int op1 = 0;
string nazwy[3] = { "Byk", "¯ubr", "Kok" };
int main()
{
    tworzenie_bazy();
    menu();

    return 0;
}
void menu() {
    setlocale(LC_CTYPE, "Polish");
    int op2 = 0;
    cout << "Witamy!" << endl << "Wybierz Bank w którym chcesz lub masz konto:" << endl;
    cout << "1. Bank Byk" << endl << "2. Bank Zubr" << endl << "3. Bank Kok" << endl << "\n0. Wyjscie" << endl;
    cin >> op1;
    while (op1 < 0 || op1>3) {
        cout << "Niestety nie ma takiej opcji" << "\nDokonaj ponownie wyboru:" << endl;
        cout << "1. Bank Byk" << endl << "2. Bank ¯ubr" << endl << "3. Bank Kok" << endl << "\n0. Wyjscie" << endl;
        cin >> op1;
    }
    if (op1 == 0)
        exit(0);
    cout << "Witamy w Banku " << nazwy[op1 - 1] << endl;
    cout << "Wybierz opcje:\n1. Zajerestruj\n2. Zaloguj\n\n0. Wroc" << endl;
    cin >> op2;
    if (op2 >= 0 && op2 < 3) {
        op2 == 1 ? rejestr() : login();
        if (op2 == 0)
            menu();
    }


}
void rejestr() {
    sqlite3* baza;
    sqlite3_stmt* stmt;
    char* error;
    int kw;
    string imie, nazwisko, pesel, email, num_tel, haslo, p_haslo;
    int wiek;
    vector<string> bledy;
    regex imiona("[^0-9]+");
    regex zDuzej("[A-Z]");
    regex popPesel("[0-9]{4}[0-3]{1}[0-9}{1}[0-9]{5}");
    regex popEmail("([_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4}))");
    regex popNumTel("[0-9]{9}");
    regex hasloDuza("[A-Z]+");
    regex hasloMala("[a-z]+");
    regex hasloLiczba("[0-9]+");
    regex hasloSpec("[@!?]+");

    cout << "Wype³nij poni¿szy formularz!\n";
    cout << "Imie: "; cin >> imie;
    cout << "Nazwisko: "; cin >> nazwisko;
    cout << "Wiek: "; cin >> wiek;
    cout << "Pesel: "; cin >> pesel;
    cout << "Adres E-Mail: "; cin >> email;
    cout << "Numer Telefonu: "; cin >> num_tel;
    cout << "Has³o"; cin >> haslo;
    cout << "Powtórz Has³o"; cin >> p_haslo;

    if (!regex_match(imie, imiona)) {
        cout << "Błąd: Liczby w imieniu" << endl;
        bledy.push_back("Błąd: Liczby w imieniu");
    }
    if (!isupper(imie[0])) {
        cout << "Błąd: Imię zaczyna się z małej litery" << endl;
        bledy.push_back("Błąd: Imię zaczyna się z małej litery");
    }
    if (!regex_match(nazwisko, imiona)) {
        cout << "Błąd: Liczby w nazwisku" << endl;
        bledy.push_back("Błąd: Liczby w nazwisku");
    }
    if (!isupper(nazwisko[0])) {
        cout << "Błąd: Nazwisko zaczyna się z małej litery" << endl;
        bledy.push_back("Błąd: Nazwisko zaczyna się z małej litery");
    }
    if (wiek < 18) {
        cout << "Błąd: Jesteś za młody!" << endl;
        bledy.push_back("Błąd: Jesteś za młody!");
    }
    if (!regex_match(pesel, popPesel)) {
        cout << "Błąd: Nieprawidłowy PESEL wprowadzony" << endl;
        bledy.push_back("Błąd: Nieprawidłowy PESEL wprowadzony");
    }
    if (!regex_match(email, popEmail)) {
        cout << "Błąd: Nieprawidłowy E-Mail wprowadzony" << endl;
        bledy.push_back("Błąd: Nieprawidłowy E-Mail wprowadzony");
    }
    if (!regex_match(num_tel, popNumTel)) {
        cout << "Błąd: Nieprawidłowy Numer Telefonu wprowadzony" << endl;
        bledy.push_back("Błąd: Nieprawidłowy Numer Telefonu wprowadzony");
    }
    if (!regex_match(haslo, hasloDuza) or !regex_match(haslo, hasloMala) or !regex_match(haslo, hasloLiczba) || !regex_match(haslo, hasloSpec)) {
        cout << "Błąd: Nieprawidłowe haslo: Przynajmniej\n1 duża litera,\n1 mała litera\n1 liczba\n1 znak specjalny" << endl;
        bledy.push_back("Błąd: Nieprawidłowe haslo");
    }
    if (p_haslo != haslo) {
        cout << "Błąd: Powtórzone Hasło inne niż Hasło" << endl;
        bledy.push_back("Błąd: Powtórzone Hasło inne niż Hasło");
    }

    if (bledy.size() > 0) {
        if (count(bledy.begin(), bledy.end(), "Błąd: Jesteś za młody!"))
            menu();
        else
            rejestr();
    }

    sqlite3_open("baza.db", &baza);
    //kw = "insert into"
    /*char* error;
    sqlite3* db;
    sqlite3_stmt* stmt;
    string query;
    int kw;
    sqlite3_open("Baza.db", &db);
    //int kw = sqlite3_exec(db,"CREATE TABLE IF NOT EXISTS bank(imie varchar(30), wiek INT);",NULL,NULL,&error);
    kw = sqlite3_exec(db, "DELETE FROM bank", NULL, NULL, &error);

    for (int i = 0; i < 5; ++i) {
        query = "insert into bank VALUES('Sxzymon', " + to_string(i) + ");";
        cout << query << endl;
        kw = sqlite3_exec(db, query.c_str(), NULL, NULL, &error);
        if (kw != SQLITE_OK) {
            cout << "Error: " << error << endl;
        }
    }
    sqlite3_prepare_v2(db, "select imie, wiek from bank", -1, &stmt, 0);
    const unsigned char* imie;
    int wiek;
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        imie = sqlite3_column_text(stmt, 0);
        wiek = sqlite3_column_int(stmt, 1);
        cout << "imie: " << imie << " Wiek: " << wiek << endl;
    }
    sqlite3_finalize(stmt);*/
    sqlite3_close(baza);

}
void login() {

}

void tworzenie_bazy() {
    sqlite3* baza;
    string kw;
    sqlite3_stmt* stmt;
    char* error;
    int sql;
    sqlite3_open("baza.db", &baza);
    kw = "CREATE TABLE IF NOT EXISTS Bank_Byk(imie varchar(30), nazwisko varchar(30), wiek INT(3), pesel varchar(20), adres varchar(30), haslo varchar(30)); ";
    kw += "CREATE TABLE IF NOT EXISTS Bank_¯ubr(imie varchar(30), nazwisko varchar(30), wiek INT(3), pesel varchar(20), adres varchar(30), haslo varchar(30));";
    kw += "CREATE TABLE IF NOT EXISTS Bank_Kok(imie varchar(30), nazwisko varchar(30), wiek INT(3), pesel varchar(20), adres varchar(30), haslo varchar(30));";
    sql = sqlite3_exec(baza, kw.c_str(), NULL, NULL, &error);
    if (sql != SQLITE_OK)
        cout << "Error: " << error << endl;
    //sqlite3_prepare_v2(baza,"select imie from Bank_¯ubr",-1,&stmt,0);
    //const unsigned char* imie;
    //sqlite3_step(stmt);
    //imie = sqlite3_column_text(stmt, 1);
    //cout << endl << "nazwisko: " << imie << endl;
    sqlite3_close(baza);
}
