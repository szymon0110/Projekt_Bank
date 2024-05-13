﻿#include <iostream>
#include <string>
#include "sqlite3/sqlite3.h"
#include <regex>
#include <vector>

using namespace std;
void menu();
void rejestr();
void login();
void tworzenie_bazy();
void czy_w_bazie(const string& pesel, int& pesel_check, const string& email, int& email_check);
int op1 = 0;
string nazwy[3] = { "Byk", "Żubr", "Kok" };
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
        if (op2 == 0)
            menu();
        op2 == 1 ? rejestr() : login();
        
    }


}
void rejestr() {
    sqlite3* baza;
    sqlite3_stmt* stmt;
    char* error;
    int kw;
    sqlite3_open("baza.db", &baza);

    string imie, nazwisko, pesel, email, num_tel, haslo, p_haslo;
    int wiek, pesel_check, email_check;
    vector<string> bledy;

    regex imionaNum("[^0-9]+");
    regex imionaSpec("[^@!?]+");
    regex zDuzej("[A-Z]");
    regex popPesel("[0-9]{11}");
    regex popEmail("([_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4}))");
    regex popNumTel("[0-9]{9}");
    regex popHaslo("(?=.*[A-Z])(?=.*[a-z])(?=.*[0-9])(?=.*[!@#$%^&*()_+\\-={}\\[\\]:;\"'<>,.?/\\\\]).{8,30}");
    regex hasloDuza("[A-Z]+");
    regex hasloMala("[a-z]+");
    regex hasloLiczba("[0-9]+");
    regex hasloSpec("[@!?]+");
    cout << "-----------------Rejestracja-----------------" << endl;
    cout << "Wypełnij poniższy formularz!\n";
    cout << "Imie: "; cin >> imie;
    cout << "Nazwisko: "; cin >> nazwisko;
    cout << "Wiek: "; cin >> wiek;
    cout << "Pesel: "; cin >> pesel;
    cout << "Adres E-Mail: "; cin >> email;
    cout << "Numer Telefonu: "; cin >> num_tel;
    cout << "Hasło: "; cin >> haslo;
    cout << "Powtórz Hasło: "; cin >> p_haslo;

    if (!regex_match(imie, imionaNum) || !regex_match(imie, imionaSpec)) {
        cout << "Błąd: Liczby w imieniu" << endl;
        bledy.push_back("Błąd: Liczby w imieniu");
    }
    if (!isupper(imie[0])) {
        cout << "Błąd: Imię zaczyna się z małej litery" << endl;
        bledy.push_back("Błąd: Imię zaczyna się z małej litery");
    }
    if (!regex_match(nazwisko, imionaNum) || !regex_match(nazwisko, imionaSpec)) {
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
    if (!regex_match(haslo, popHaslo)) {
        cout << "Błąd: Nieprawidłowe haslo: Przynajmniej\n1 duża litera,\n1 mała litera\n1 liczba\n1 znak specjalny\nMin 8 Znaków\nMaks 30 znaków" << endl;
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
    else if (bledy.size() == 0) {
        czy_w_bazie(pesel, pesel_check, email, email_check);
        if (email_check != 0)
            cout << "Konto z tym emailem jest już zarejestrowane!" << endl;
        if (pesel_check != 0) {
            cout << "Konto z tym peselem jest już zarejestrowane!" << endl;
            
        }
        else{

            string kwerenda = "INSERT INTO Bank_"+nazwy[op1-1] + " VALUES('" + imie + "', '" + nazwisko + "', " + to_string(wiek) + ", '" + pesel + "', '" + email + "', '" + num_tel + "', '" + haslo + "');";

            int kw = sqlite3_exec(baza, kwerenda.c_str(), NULL, NULL, &error);

            if (kw != SQLITE_OK) {
                cout << "Error: " << error << endl;
            }
            else
                cout << "Pomyślnie utworzono konto w Banku "+nazwy[op1-1]+" !" << endl;
        }
    }
    sqlite3_close(baza);
    menu();
    
    /*
    kw = "insert into"
    char* error;
    sqlite3* db;
    sqlite3_stmt* stmt;
    string query;
    int kw;
    sqlite3_open("Baza.db", &db);
    int kw = sqlite3_exec(db,"CREATE TABLE IF NOT EXISTS bank(imie VARCHAR(30), wiek INT);",NULL,NULL,&error);
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
    sqlite3_finalize(stmt);
    */
}
void login() {
    sqlite3* baza;
    string kw;
    sqlite3_stmt* stmt;
    char* error = nullptr;
    string email;
    string haslo;
    int login_check = 0;
again:
    cout << "-----------------LOGIN-----------------" << endl;
    cout << "E-Mail: ";cin >> email;
    cout << "Hasło: ";cin >> haslo;
    
    sqlite3_open("baza.db", &baza);
    kw = "Select COUNT(*) FROM Bank_"+nazwy[op1-1]+ " WHERE email = '" + email + "' AND haslo = '" + haslo + "';";
    if (sqlite3_prepare_v2(baza, kw.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            login_check = sqlite3_column_int(stmt,0);
            if (login_check == 0) {
                cout << "E-Mail lub hasło jest nieprawidłowe\nSpróbuj jeszcze raz!" << endl;
                goto again;
            }
            else
                cout << "Witamy ponownie w Banku "+nazwy[op1-1] +" !"<< endl;
        }
        sqlite3_finalize(stmt);
    }
    else {
        cerr << "Error z preparev2: " << sqlite3_errmsg(baza) << endl;
    }
    


}


/*
int czy_w_bazie(const string& pesel) {
    sqlite3* baza;
    char* error = nullptr;
    int result = -1; // Initialize result to an error value

    if (sqlite3_open("baza.db", &baza) == SQLITE_OK) { // Check if database opened successfully
        // Construct the query string
        string query = "SELECT COUNT(*) FROM Bank_Byk WHERE pesel = ?;";


        // Execute the query
        if (sqlite3_exec(baza, query.c_str(), NULL, NULL, &error) == SQLITE_OK) {
            result = sqlite3_changes(baza); // Get the result directly
        }
        else {
            cerr << "Error executing query: " << error << std::endl;
            sqlite3_free(error);
        }

        sqlite3_close(baza); // Close the database connection
    }
    else {
        cerr << "Error opening database" << std::endl;
    }

    return result;
}*/
void czy_w_bazie(const string& pesel, int& pesel_check, const string& email, int& email_check) {
    sqlite3* baza;
    char* error = nullptr;
    

    if (sqlite3_open("baza.db", &baza) == SQLITE_OK){
        string kw = "SELECT COUNT(*) FROM Bank_"+nazwy[op1-1] + " WHERE pesel = '" + pesel + "';";
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(baza, kw.c_str(), -1, &stmt, nullptr) == SQLITE_OK){
            if (sqlite3_step(stmt) == SQLITE_ROW){
                pesel_check = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
        else
            cerr << "Error pesel check: " << sqlite3_errmsg(baza) << endl;
        
        kw = "SELECT COUNT(*) FROM Bank_"+nazwy[op1-1] + " WHERE email = '" + email + "';";
        if (sqlite3_prepare_v2(baza, kw.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                email_check = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
        else {
            cerr << "Error email check: " << sqlite3_errmsg(baza) << endl;
        }

        sqlite3_close(baza);
    }
    else {
        cerr << "Error opening database" << endl;
    }

    
}
void tworzenie_bazy() {
    sqlite3* baza;
    string kw;
    char* error;
    int sql;
    sqlite3_open("baza.db", &baza);
    kw = "CREATE TABLE IF NOT EXISTS Bank_Byk(imie VARCHAR(30), nazwisko VARCHAR(30), wiek INT(3), pesel VARCHAR(11), email VARCHAR(50), telefon VARCHAR(9), haslo VARCHAR(30));";
    kw += "\nCREATE TABLE IF NOT EXISTS Bank_Żubr(imie VARCHAR(30), nazwisko VARCHAR(30), wiek INT(3), pesel VARCHAR(11), email VARCHAR(50), telefon VARCHAR(9), haslo VARCHAR(30));";
    kw += "\nCREATE TABLE IF NOT EXISTS Bank_Kok(imie VARCHAR(30), nazwisko VARCHAR(30), wiek INT(3), pesel VARCHAR(11), email VARCHAR(50), telefon VARCHAR(9), haslo VARCHAR(30));";
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