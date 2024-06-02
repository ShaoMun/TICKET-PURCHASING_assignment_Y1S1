#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <windows.h>
#include <stdlib.h>

#define ROWS 14
#define COLS 20
#define REG_PRICE 75.0
#define EXC_PRICE 125.0
#define VIP_PRICE 250.0

using namespace std;


struct ticketType
{
    string seat = "\0";
    int seatArray[2] = { 0 };
    double price = 0.0;
    int serialNum = 0;
};

struct userType
{
    string username;
    string password;
    int ticket_bought[280] = { 0 };
    int ticket_quantity = 0;
};

ticketType Ticket[280];
userType User[280];

void signup(userType[], int&);
bool login(userType[], int, int&);
bool signup_login(userType[], int&, int&);
void displayMenu();
void initialSeats(string[][COLS]);
void displaySeats(string[][COLS]);
void displayAbout();
void genSerialNum(ticketType[], int, userType[], int);
void bookSeats(string[][COLS], int&, int&, double&, ticketType[], bool&, int&, int&, int&, int&, int&, int&, userType[], int);
void DisplayPrice();
char confirm_payment(double);
bool card();
bool onlineBanking(double, double&, double&, double&);
bool eWallet(double, double&, double&, double&);
bool paymentStatus(double, double&, double&, double&, double&, double&, double&);
void recoverSeats(ticketType[], int&, int, string[][COLS], int&, int&, int&, int, int, int, userType[], int);
void displayTicket(ticketType[], int);
void check_serialNum(ticketType[]);
void check_user(userType[], ticketType[]);
void checkTicket(userType[], ticketType[]);
void getFeedback(string[], int&);
void printFeedback(string[], int&);
bool AdminLogin();
void displayStaffMenu();
bool CancelConcert();
bool suspendTicketing();
bool reactivateTicketing();
void salesReport(int, double, int, int, int);
void staffMenu(bool&, bool&, string[], int&, int, double, int, int, int);




int main() {
    char letter;
    int total_seatCount = 0, totalVipSeat = 0, totalExcSeat = 0, totalRegSeat = 0, feedbackCount = 0, userCount = 0;
    double total_revenue = 0, bank_balance1 = 5000.00, bank_balance2 = 5000.00, bank_balance3 = 5000.00, eWallet_balance1 = 5000.00, eWallet_balance2 = 5000.00, eWallet_balance3 = 5000.00;
    bool ticketSuspend = false, concertCancelled = false;

    string seats[ROWS][COLS] = { "" }, feedback[280] = { "" };


    initialSeats(seats);

    do { //let the system keep looping
        int temp_seatCount = 0;
        double total_price = 0;
        int temp_VipSeat = 0, temp_ExcSeat = 0, temp_RegSeat = 0;
        int current_user = 0;
        bool bookStatus = false;

        displayMenu();

        cout << "\n\nSelect option: ";
        cin >> letter;

        if (isupper(letter) != 0)
        {
            letter = static_cast<char>(tolower(letter));
        }

        switch (letter) {
        case 'a':
        {
            if (total_seatCount == 280)
            {
                cout << "All seats are fully booked." << endl;
                cout << "Press enter to continue......" << endl;
                cin.ignore();
                cin.get();
                break;
            }
            else if (ticketSuspend == true) {
                cout << "Ticketing process is currently suspended. We apologize for any inconvenience caused." << endl;
                cout << "Press enter to continue......" << endl;
                cin.ignore();
                cin.get();
                break;
            }
            else
            {
                if (signup_login(User, userCount, current_user) == false)
                {
                    break;
                }


                bookSeats(seats, total_seatCount, temp_seatCount, total_price, Ticket, bookStatus, totalVipSeat, totalExcSeat, totalRegSeat, temp_VipSeat, temp_ExcSeat, temp_RegSeat, User, current_user);

                if (bookStatus == true)
                {
                    //proceed to payment part
                    if (confirm_payment(total_price) == 'Y')
                    {
                        char confirmRetry;
                        do
                        {
                            confirmRetry = '\0';
                            if (paymentStatus(total_price, bank_balance1, bank_balance2, bank_balance3, eWallet_balance1, eWallet_balance2, eWallet_balance3) == true)
                            {
                                system("cls");
                                cout << "Payment Successful" << endl;
                                total_revenue += total_price;
                                //displaying ticket
                                for (int i = 1; i <= temp_seatCount; i++)
                                {
                                    cout << "Ticket " << i << endl;
                                    displayTicket(Ticket, total_seatCount - i);
                                    cout << "\n\n";
                                }
                                cout << "Press enter to continue......" << endl;
                                cin.get();

                            }
                            else
                            {
                                do
                                {
                                    cout << "Payment Unsuccessful." << endl;
                                    cout << "Do you want to retry?(Y/N): ";
                                    cin >> confirmRetry;
                                    //data validation
                                    if (islower(confirmRetry) != 0)
                                    {
                                        confirmRetry = static_cast<char>(toupper(confirmRetry));
                                    }
                                } while (confirmRetry != 'Y' && confirmRetry != 'N');

                                if (confirmRetry == 'N')
                                    recoverSeats(Ticket, total_seatCount, temp_seatCount, seats, totalVipSeat, totalExcSeat, totalRegSeat, temp_VipSeat, temp_ExcSeat, temp_RegSeat, User, current_user);
                            }
                        } while (confirmRetry == 'Y');
                    }
                    else
                        recoverSeats(Ticket, total_seatCount, temp_seatCount, seats, totalVipSeat, totalExcSeat, totalRegSeat, temp_VipSeat, temp_ExcSeat, temp_RegSeat, User, current_user);
                }

                break;
            }
        }
        case 'b':
            DisplayPrice();
            cout << "Press enter to continue......" << endl;
            cin.ignore();
            cin.get();
            break;
        case 'c':
            displaySeats(seats);
            cout << "Press enter to continue......" << endl;
            cin.ignore();
            cin.get();
            break;
        case 'd':
            displayAbout();
            cout << "Press enter to continue......" << endl;
            cin.ignore();
            cin.get();
            break;
        case 'e':
            checkTicket(User, Ticket);
            break;
        case 'f':
            getFeedback(feedback, feedbackCount);
            cout << "Press enter to continue......" << endl;
            cin.get();
            break;
        case 'g':
            signup(User, userCount);
            cout << "Press enter to continue......" << endl;
            cin.get();
            break;
        case 'h':
            if (AdminLogin()) {
                staffMenu(ticketSuspend, concertCancelled, feedback, feedbackCount, total_seatCount, total_revenue, totalVipSeat, totalExcSeat, totalRegSeat);
                cout << "Press enter to continue......" << endl;
                cin.get();
            }
            else {
                cout << "Invalid username or password." << endl;
                cout << "Press enter to continue......" << endl;
                cin.get();
            }
            break;
        default:
            cout << "Please Enter A Valid Option From Screen (e.g. a)" << endl;
            cout << "Press enter to continue......" << endl;
            cin.ignore();
            cin.get();
        }

    } while (concertCancelled == false);
    return 0;
}



//signup
void signup(userType User[], int& userCount)
{
    system("cls");
    string input_username, input_password, retype_password;

    cin.get();
    cout << "Enter your username: ";
    getline(cin, input_username);
    cout << "Enter your password: ";
    getline(cin, input_password);
    cout << "Retype your password: ";
    getline(cin, retype_password);
    if (retype_password == input_password)
    {
        system("cls");
        cout << "Sign up successful. Please log in to purchase ticket." << endl;
        User[userCount].username = input_username;
        User[userCount].password = input_password;
        userCount++;
    }
    else
    {
        system("cls");
        cout << "Password not matched. Please try again." << endl;
    }
}
//login
bool login(userType User[], int userCount, int& current_user)
{
    system("cls");
    string input_username, input_password;

    bool login_status = false;
    cin.get();
    cout << "Username: ";
    getline(cin, input_username);
    cout << "Password: ";
    getline(cin, input_password);
    for (int i = 0; i < userCount; i++)
    {
        if (input_username == User[i].username && input_password == User[i].password)
        {
            login_status = true;
            current_user = i;
            break;
        }
    }
    if (login_status == true)
    {
        system("cls");
        cout << "Login successful." << endl;
    }

    else
    {
        system("cls");
        cout << "Login unsuccessful. Please try again." << endl;
    }
    return login_status;

}
bool signup_login(userType User[], int& userCount, int& current_user)
{
    bool login_status = false;
    bool registration = true;
    do
    {
        system("cls");
        char selection;
        cout << "a) Sign Up" << endl;
        cout << "b) Log In" << endl;
        cout << "c) Exit" << endl;
        cout << "Select(e.g. a) >> ";
        cin >> selection;
        //data validation
        if (isupper(selection) != 0)
        {
            selection = static_cast<char>(tolower(selection));
        }
        switch (selection)
        {
        case 'a':
            signup(User, userCount);
            cout << "Press enter to continue......" << endl;
            cin.get();
            break;
        case 'b':
            login_status = login(User, userCount, current_user);
            cout << "Press enter to continue......" << endl;
            cin.get();
            break;
        case 'c':
            registration = false;
            break;
        default:
            cout << "Invalid selection. Please try again." << endl;
            cout << "Press enter to continue......" << endl;
            cin.ignore();
            cin.get();
        }
    } while (login_status == false && registration == true);

    return registration;
}


//FUnction to display menu
void displayMenu() {
    system("cls");
    cout << "  ______            _ ______  _                 ______                                  \n";
    cout << " / _____)          | (_____ \\| |               / _____)                            _    \n";
    cout << "| /      ___   ___ | |_____) ) | ____ _   _   | /      ___  ____   ____ ____  ____| |_  \n";
    cout << "| |     / _ \\ / _ \\| |  ____/| |/ _  | | | |  | |     / _ \\|  _ \\ / ___) _  )/ ___)  _) \n";
    cout << "| \\____| |_| | |_| | | |     | ( ( | | |_| |  | \\____| |_| | | | ( (__| (/ /| |   | |__ \n";
    cout << " \\______)___/ \\___/|_|_|     |_|\\_||_|\\__  |   \\______)___/|_| |_|\\____)____)_|    \\___)\n";
    cout << "                                      (____/                                              \n";
    cout << "\n\n\na) Purchase Tickets" << endl;
    cout << "b) Seats Prices" << endl;
    cout << "c) Check Available Seats" << endl;
    cout << "d) About" << endl;
    cout << "e) Check Ticket" << endl;
    cout << "f) Feedback" << endl;
    cout << "g) Sign Up" << endl;
    cout << "h) Staff Login" << endl;
}

//function for initializing the seats array
void initialSeats(string seats[][COLS])
{
    char rowLabel = 'A';

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
            seats[i][j] = rowLabel + to_string(j + 1);

        rowLabel++;
    }
}


//function for displaying seats
void displaySeats(string seats[][COLS])
{
    system("cls");
    cout << setw(80) << setfill(' ') << endl;
    cout << "================================================================================" << endl;
    cout << setw(78) << left << "||" << right << "||" << endl;
    cout << setw(78) << left << "||" << right << "||" << endl;
    cout << setw(36) << left << "||" << right << "STAGE" << setw(39) << "||" << endl;
    cout << setw(78) << left << "||" << right << "||" << endl;
    cout << setw(78) << left << "||" << right << "||" << endl;
    cout << "================================================================================\n";
    cout << setw(48) << "  _   _________ \n";
    cout << setw(48) << " | | / /  _/ _ \\\n";
    cout << setw(48) << " | |/ // // ___/\n";
    cout << setw(48) << " |___/___/_/    \n";
    cout << setw(48) << "(Price: RM250.00)" << endl << endl;
    for (int i = 0; i < ROWS; i++)
    {
        if (i == 2) {
            cout << "\n";
            cout << setw(65) << "   _____  _________   __  ___________   ______\n";
            cout << setw(65) << "  / __/ |/_/ ___/ /  / / / / __/  _/ | / / __/\n";
            cout << setw(65) << " / _/_>  </ /__/ /__/ /_/ /\\ \\_/ / | |/ / _/  \n";
            cout << setw(65) << "/___/_/|_|\\___/____/\\____/___/___/ |___/___/  \n";
            cout << setw(48) << "(Price: RM125.00)" << endl << endl;
        }
        else if (i == 6)
        {
            cout << "\n";
            cout << setw(60) << "   ___  ___________  ____   ___   ___ \n";
            cout << setw(60) << "  / _ \\/ __/ ___/ / / / /  / _ | / _ \\ \n";
            cout << setw(60) << " / , _/ _// (_ / /_/ / /__/ __ |/ , _/ \n";
            cout << setw(60) << "/_/|_/___/\\___/\\____/____/_/ |_/_/|_|  \n";
            cout << setw(48) << "(Price: RM75.00)" << endl << endl;
        }
        for (int j = 0; j < COLS; j++) {
            cout << setw(3) << seats[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}


//function for displaying about
void displayAbout()
{
    system("cls");
    cout << "COOLPLAY Concert Details" << endl;
    cout << "========================" << endl;
    cout << setw(15) << left << "Date" << ": 5th December 2023" << endl;
    cout << setw(15) << left << "Time" << ": 8:00p.m. to 10:00p.m." << endl;
    cout << setw(15) << left << "Venue" << ": ABC Concert Hall" << endl;
}

//Function to generate serial number for rechecking purpose
void genSerialNum(ticketType Ticket[], int total_seatCount, userType User[], int current_user)
{
    srand(time_t(NULL));
    Ticket[total_seatCount].serialNum = rand() % 10000000 + 80000000;
    //prevent serial number repeated
    for (int i = 0; i < total_seatCount; i++)
    {
        if (Ticket[total_seatCount].serialNum == Ticket[i].serialNum)
        {
            ++Ticket[total_seatCount].serialNum;
        }
    }
    User[current_user].ticket_bought[User[current_user].ticket_quantity] = Ticket[total_seatCount].serialNum;
}

//function for booking seats
void bookSeats(string seats[][COLS], int& total_seatCount, int& temp_seatCount, double& total_price, ticketType Ticket[], bool& bookStatus, int& totalVipSeat, int& totalExcSeat, int& totalRegSeat, int& temp_VipSeat, int& temp_ExcSeat, int& temp_RegSeat, userType User[], int current_user)
{
    char buyMore = '\0';

    do
    {
        string bookedSeats;
        bool foundNum = false;

        displaySeats(seats);
        cout << "Select your seat(e.g. A1): ";
        getline(cin, bookedSeats);

        //data validation
        if (islower(bookedSeats.at(0)) != 0)
        {
            bookedSeats.at(0) = static_cast<char>(toupper(bookedSeats.at(0)));
        }

        for (int i = 0; i < ROWS; i++)
        {
            for (int j = 0; j < COLS; j++)
            {
                if (bookedSeats == seats[i][j])
                {
                    foundNum = true;
                    seats[i][j] = "n/a";
                    displaySeats(seats);

                    switch (bookedSeats.at(0))
                    {
                    case 'A':
                    case 'B':
                        Ticket[total_seatCount].price = VIP_PRICE;
                        ++temp_VipSeat;
                        ++totalVipSeat;
                        break;
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                        Ticket[total_seatCount].price = EXC_PRICE;
                        ++temp_ExcSeat;
                        ++totalExcSeat;
                        break;
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                        Ticket[total_seatCount].price = REG_PRICE;
                        ++temp_RegSeat;
                        ++totalRegSeat;
                        break;
                    }

                    bookStatus = true;
                    total_price += Ticket[total_seatCount].price;
                    Ticket[total_seatCount].seatArray[0] = i;
                    Ticket[total_seatCount].seatArray[1] = j;
                    Ticket[total_seatCount].seat = bookedSeats;
                    genSerialNum(Ticket, total_seatCount, User, current_user);
                    ++User[current_user].ticket_quantity;
                    total_seatCount++;
                    temp_seatCount++;
                    break;
                }
            }
        }

        if (foundNum == false)
        {
            cout << "Invalid seat selection." << endl;
        }

        do //confirm users type Y or N
        {
            cout << "Do you want to buy another seat? (Y/N): ";
            cin >> buyMore;
            cin.get();

            //data validation
            if (islower(buyMore) != 0)
            {
                buyMore = static_cast<char>(toupper(buyMore));
            }

            if (buyMore != 'Y' && buyMore != 'N')
                cout << "Invalid selection." << endl;
        } while (buyMore != 'Y' && buyMore != 'N');
    } while (buyMore == 'Y');

}

// Function to display seat prices
void DisplayPrice() {
    system("cls");

    string SeatCategory[] = { "VIP (Row A-D)     ", "Exclusive (Row E-L) ", "Regular (Row M-Z)" };
    double Prices[] = { 250.0, 125.0, 75.0 };
    int NumSeatCategories = 3;

    cout << "Concert Seat Price Table\n__________________________________" << endl;
    cout << setw(4) << "Category" << setw(25) << "Price" << endl;
    cout << "----------------------------------" << endl;

    for (int i = 0; i < NumSeatCategories; ++i) {
        cout << setw(24) << left << SeatCategory[i] << setw(6) << right << "RM" << Prices[i] << endl;
    }

    cout << "----------------------------------" << endl;
}


//Banking system
//Functionn to confirm whether user want to proceed to payment or not
char confirm_payment(double ticketTotal) {
    system("cls");
    char paymentAnswer;

    do
    {
        cout << "The total price is RM" << fixed << setprecision(2) << ticketTotal << endl;
        cout << "Do you want to proceed to payment?(Y/N) ";
        cin >> paymentAnswer;
        if (islower(paymentAnswer) != 0)
        {
            paymentAnswer = static_cast<char>(toupper(paymentAnswer));
        }

        if (paymentAnswer != 'Y' && paymentAnswer != 'N')
        {
            system("cls");
            cout << "Invalid selection. Please retry." << endl;
        }
    } while (paymentAnswer != 'Y' && paymentAnswer != 'N');

    return paymentAnswer;
}
//Function for Credit/Dedit card login
bool card() {
    system("cls");
    cin.get();

    bool paymentStatus = false;
    string cardNo, expiryDate, cardCVV;
    srand(time_t(NULL));
    int otp = rand() % 900000 + 100000;
    int otp_validation = 0;

    cout << " __   ___              __  __  __         _                          _ \n";
    cout << " \\ \\ / (_)___ __ _    / / |  \\/  |__ _ __| |_ ___ _ _ __ __ _ _ _ __| |\n";
    cout << "  \\ V /| (_-</ _` |  / /  | |\\/| / _` (_-<  _/ -_) '_/ _/ _` | '_/ _` |\n";
    cout << "   \\_/ |_/__/\\__,_| /_/   |_|  |_|\\__,_/__|\\__\\___|_| \\__\\__,_|_| \\__,_|\n";
    cout << "                                                                        \n";
    cout << "\n\n";
    cout << "\t\tCard No.    (E.g xxxx xxxx xxxx): ";
    getline(cin, cardNo);
    cout << "\t\tExpiry Date (E.g xxxx)          : ";
    getline(cin, expiryDate);
    cout << "\t\tCard CVV    (E.gxxx)            : ";
    getline(cin, cardCVV);
    if (cardNo == "789456123789" && expiryDate == "0824" && cardCVV == "857")
    {
        cout << "\n" << otp << " is your OTP. DO NOT share it." << endl;
        cout << "\nPlease enter your OTP >> ";
        cin >> otp_validation;
        if (otp_validation == otp)
            paymentStatus = true;
    }
    else if (cardNo == "123456789123" && expiryDate == "0724" && cardCVV == "369")
    {
        cout << "\n" << otp << " is your OTP. DO NOT share it." << endl;
        cout << "\nPlease enter your OTP >> ";
        cin >> otp_validation;
        if (otp_validation == otp)
            paymentStatus = true;
    }
    else if (cardNo == "987654321987" && expiryDate == "0624" && cardCVV == "147")
    {
        cout << "\n" << otp << " is your OTP. DO NOT share it." << endl;
        cout << "\nPlease enter your OTP >> ";
        cin >> otp_validation;
        if (otp_validation == otp)
            paymentStatus = true;
    }
    else
        cout << "Invalid card information." << endl;

    return paymentStatus;
}
//Function for Online Banking login
bool onlineBanking(double total_price, double& bank_balance1, double& bank_balance2, double& bank_balance3) {
    system("cls");
    cin.get();

    string bank_username, bank_password;
    int bank_selection = 0;
    srand(time_t(NULL));
    int otp = rand() % 900000 + 100000;
    int otp_validation = 0;
    bool paymentStatus = false;
    //get the current system time
    time_t now = _time64(nullptr);
    tm buyTime;
    _localtime64_s(&buyTime, &now);

    cout << "1. Maybank" << endl;
    cout << "2. Public Bank" << endl;
    cout << "3. CIMB Bank" << endl;
    cout << "Please select bank >> ";
    cin >> bank_selection;

    switch (bank_selection)
    {
    case 1:
        system("cls");
        cout << "  __  __           _               _   \n";
        cout << " |  \\/  |__ _ _  _| |__  __ _ _ _ | |__\n";
        cout << " | |\\/| / _` | || | '_ \\/ _` | ' \\| / /\n";
        cout << " |_|  |_|\\__,_|\\_, |_|_|\\_\\__,_|_||_|_\\_\\\n";
        cout << "              |__/\n";
        cout << "\n\n";
        cout << "\t\tUsername: ";
        cin >> bank_username;
        cout << "\t\tPassword: ";
        cin >> bank_password;
        break;
    case 2:
        system("cls");
        cout << "  ___      _    _ _      ___            _   \n";
        cout << " | _ \\_  _| |__| (_)__  | _ \\ __ _ _ _ | |__\n";
        cout << " |  _/ || | '_ \\ | / _| | _ \\/ _` | ' \\| / /\n";
        cout << " |_| \\_,_|_.__/_|_\\__| |___/\\__,_|_||_|_\\_\\\n";
        cout << "                                            \n";
        cout << "\n\n";
        cout << "\t\tUsername: ";
        cin >> bank_username;
        cout << "\t\tPassword: ";
        cin >> bank_password;
        break;
    case 3:
        system("cls");
        cout << "   ___ ___ __  __ ___   ___            _   \n";
        cout << "  / __|_ _|  \\/  | _ ) | _ ) __ _ _ _ | |__\n";
        cout << " | (__ | || |\\/| | _ \\ | _ \\/ _` | ' \\| / /\n";
        cout << "  \\___|___|_|  |_|___/ |___/\\__,_|_||_|_\\_\\\n";
        cout << "                                            \n";
        cout << "\n\n";
        cout << "\t\tUsername: ";
        cin >> bank_username;
        cout << "\t\tPassword: ";
        cin >> bank_password;
        break;
    default:
        cout << "Invalid bank selection." << endl;
    }

    if (bank_selection == 1 && bank_username == "gavin123" && bank_password == "123gavin123")
    {
        if (bank_balance1 < total_price)
            cout << "Balance insufficient." << endl;
        else
        {
            cout << "Pay to       : ABC Sdn Bhd" << endl;
            cout << "Date         : " << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;
            cout << "Amount to pay: RM" << fixed << setprecision(2) << total_price << endl;

            cout << "\nPress enter to request OTP......";
            cin.ignore();
            cin.get();
            cout << otp << " is your OTP. DO NOT share it." << endl;
            cout << "\nPlease enter your OTP >> ";
            cin >> otp_validation;
            if (otp_validation == otp)
            {
                paymentStatus = true;

                bank_balance1 -= total_price;
                //print receipt
                system("cls");
                cout << "PAYMENT SUCCESSFUL" << endl;
                cout << "Pay to         : ABC Sdn Bhd" << endl;
                cout << "Date           : " << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;
                cout << "Amount paid    : RM" << fixed << setprecision(2) << total_price << endl;
                cout << "Account balance: RM" << fixed << setprecision(2) << bank_balance1 << endl;

                cout << "\nPress enter to continue......" << endl;
                cin.ignore();
                cin.get();
            }
        }
    }
    else if (bank_selection == 2 && bank_username == "joel123" && bank_password == "123joel123")
    {
        if (bank_balance2 < total_price)
            cout << "Balance insufficient." << endl;
        else
        {
            cout << "Pay to       : ABC Sdn Bhd" << endl;
            cout << "Date         : " << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;
            cout << "Amount to pay: RM" << fixed << setprecision(2) << total_price << endl;

            cout << "\nPress enter to request OTP......";
            cin.ignore();
            cin.get();
            cout << otp << " is your OTP. DO NOT share it." << endl;
            cout << "Please enter your OTP >> ";
            cin >> otp_validation;
            if (otp_validation == otp)
            {
                paymentStatus = true;

                bank_balance2 -= total_price;
                //print receipt
                system("cls");
                cout << "PAYMENT SUCCESSFUL" << endl;
                cout << "Pay to         : ABC Sdn Bhd" << endl;
                cout << "Date           : " << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;
                cout << "Amount paid    : RM" << fixed << setprecision(2) << total_price << endl;
                cout << "Account balance: RM" << fixed << setprecision(2) << bank_balance2 << endl;

                cout << "\nPress enter to continue......" << endl;
                cin.ignore();
                cin.get();
            }
        }
    }
    else if (bank_selection == 3 && bank_username == "john123" && bank_password == "123john123")
    {
        if (bank_balance3 < total_price)
            cout << "Balance insufficient." << endl;
        else
        {
            cout << "Pay to       : ABC Sdn Bhd" << endl;
            cout << "Date         : " << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;
            cout << "Amount to pay: RM" << fixed << setprecision(2) << total_price << endl;

            cout << "\nPress enter to request OTP......";
            cin.ignore();
            cin.get();
            cout << otp << " is your OTP. DO NOT share it." << endl;
            cout << "\nPlease enter your OTP >> ";
            cin >> otp_validation;
            if (otp_validation == otp)
            {
                paymentStatus = true;

                bank_balance3 -= total_price;
                //print receipt
                system("cls");
                cout << "PAYMENT SUCCESSFUL" << endl;
                cout << "Pay to         : ABC Sdn Bhd" << endl;
                cout << "Date           : " << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;
                cout << "Amount paid    : RM" << fixed << setprecision(2) << total_price << endl;
                cout << "Account balance: RM" << fixed << setprecision(2) << bank_balance3 << endl;

                cout << "\nPress enter to continue......" << endl;
                cin.ignore();
                cin.get();
            }
        }
    }
    else
        cout << "Incorrect username or password." << endl;

    return paymentStatus;

}
//Function for E-wallet login
bool eWallet(double total_price, double& eWallet_balance1, double& eWallet_balance2, double& eWallet_balance3) {
    system("cls");
    cin.get();

    bool paymentStatus = false;
    string phoneNum, pinNum;
    //get the current system time
    time_t now = _time64(nullptr);
    tm buyTime;
    _localtime64_s(&buyTime, &now);

    cout << "  _______               _                _____              \n";
    cout << " |__   __|             | |       ___    |  __ \\             \n";
    cout << "    | | ___  _   _  ___| |__    ( _ )   | |__) |   _ _ __   \n";
    cout << "    | |/ _ \\| | | |/ __| '_ \\   / _ \\/\\ |  _  / | | | '_ \\  \n";
    cout << "    | | (_) | |_| | (__| | | | | (_>  < | | \\ \\ |_| | | | | \n";
    cout << "    |_|\\___/ \\__,_|\\___|_| |_|  \\___/\\/ |_|  \\_\\__,_|_| |_| \n";
    cout << "                                                            \n";
    cout << "\nTransfer to  : ABC Sdn Bhd" << endl;
    cout << "Amount to pay: RM" << fixed << setprecision(2) << total_price << endl;
    cout << "\t\tPhone number: +60" << ":";
    cin >> phoneNum;
    if (phoneNum == "0126541234")
    {
        cout << "\t\tPIN number: ";
        cin >> pinNum;
        if (pinNum == "123456")
        {
            if (eWallet_balance1 < total_price)
                cout << "Insufficient balance. Please top up." << endl;
            else
            {
                paymentStatus = true;

                eWallet_balance1 -= total_price;
                //print receipt
                system("cls");
                cout << "  _      ___  _  \n";
                cout << " |_) /\\   |  | \\ \n";
                cout << " |  /--\\ _|_ |_/\n\n";
                cout << setw(20) << left << "Transfer to" << setw(19) << right << "ABC Sdn Bhd" << endl;
                cout << setw(20) << left << "Amount paid(RM)" << setw(19) << right << fixed << setprecision(2) << total_price << endl;
                cout << setw(20) << left << "Date" << setw(10) << right << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;

                cout << "\nPress enter to continue......";
                cin.ignore();
                cin.get();
            }
        }
        else
            cout << "Incorrect PIN number." << endl;
    }
    else if (phoneNum == "0121236789")
    {
        cout << "\t\tPIN number: ";
        cin >> pinNum;
        if (pinNum == "456789")
        {
            if (eWallet_balance2 < total_price)
                cout << "Insufficient balance. Please top up." << endl;
            else
            {
                paymentStatus = true;

                eWallet_balance2 -= total_price;
                //print receipt
                system("cls");
                cout << "  _      ___  _  \n";
                cout << " |_) /\\   |  | \\ \n";
                cout << " |  /--\\ _|_ |_/\n\n";
                cout << setw(20) << left << "Transfer to" << setw(19) << right << "ABC Sdn Bhd" << endl;
                cout << setw(20) << left << "Amount paid(RM)" << setw(19) << right << fixed << setprecision(2) << total_price << endl;
                cout << setw(20) << left << "Date" << setw(10) << right << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;

                cout << "\nPress enter to continue......";
                cin.ignore();
                cin.get();
            }
        }
        else
            cout << "Incorrect PIN number." << endl;
    }
    else if (phoneNum == "0124564321")
    {
        cout << "\t\tPIN number: ";
        cin >> pinNum;
        if (pinNum == "123789")
        {
            if (eWallet_balance3 < total_price)
                cout << "Insufficient balance. Please top up." << endl;
            else
            {
                paymentStatus = true;

                eWallet_balance3 -= total_price;
                //print receipt
                system("cls");
                cout << "  _      ___  _  \n";
                cout << " |_) /\\   |  | \\ \n";
                cout << " |  /--\\ _|_ |_/\n\n";
                cout << setw(20) << left << "Transfer to" << setw(19) << right << "ABC Sdn Bhd" << endl;
                cout << setw(20) << left << "Amount paid(RM)" << setw(19) << right << fixed << setprecision(2) << total_price << endl;
                cout << setw(20) << left << "Date" << setw(10) << right << put_time(&buyTime, "%Y-%m-%d %H:%M:%S") << endl;

                cout << "\nPress enter to continue......";
                cin.ignore();
                cin.get();
            }
        }
        else
            cout << "Incorrect PIN number." << endl;
    }
    else
        cout << "Unregistered phone number." << endl;

    return paymentStatus;


}
bool paymentStatus(double total_price, double& bank_balance1, double& bank_balance2, double& bank_balance3, double& eWallet_balance1, double& eWallet_balance2, double& eWallet_balance3)
{
    system("cls");
    int payment_method;
    bool paymentStatus = false;

    cout << "PAYMENT METHOD:" << endl;
    cout << "01. CREDIT/DEBIT CARD" << endl;
    cout << "02. ONLINE BANKING" << endl;
    cout << "03. E-WALLET" << endl;
    cout << "Choose a Payment Method(e.g. 1): ";
    cin >> payment_method;

    switch (payment_method)
    {
    case 1:
        paymentStatus = card();
        cin.ignore();
        break;
    case 2:
        paymentStatus = onlineBanking(total_price, bank_balance1, bank_balance2, bank_balance3);
        break;
    case 3:
        paymentStatus = eWallet(total_price, eWallet_balance1, eWallet_balance2, eWallet_balance3);
        break;
    default:
        cout << "Invalid payment method." << endl << endl;
    }

    return paymentStatus;

}


//function for recovering  seats if payment failed
void recoverSeats(ticketType Ticket[], int& total_seatCount, int temp_seatCount, string seats[][COLS], int& totalVipSeat, int& totalExcSeat, int& totalRegSeat, int temp_VipSeat, int temp_ExcSeat, int temp_RegSeat, userType User[], int current_user)
{
    for (int i = 1; i <= temp_seatCount; i++)
    {
        seats[Ticket[total_seatCount - i].seatArray[0]][Ticket[total_seatCount - i].seatArray[1]] = Ticket[total_seatCount - i].seat;
        Ticket[total_seatCount - i].seat = "\0";
        Ticket[total_seatCount - i].seatArray[0] = 0;
        Ticket[total_seatCount - i].seatArray[1] = 0;
        Ticket[total_seatCount - i].serialNum = 0;
        User[current_user].ticket_bought[User[current_user].ticket_quantity - i] = 0;
    }
    User[current_user].ticket_quantity -= temp_seatCount;
    totalVipSeat -= temp_VipSeat;
    totalExcSeat -= temp_ExcSeat;
    totalRegSeat -= temp_RegSeat;
    total_seatCount -= temp_seatCount;
}


//Function for displaying ticket
void displayTicket(ticketType Ticket[], int ticketIndex)
{
    cout << "========================================================" << endl;
    cout << "||                                                    ||" << endl;
    cout << "||            COOLPLAY CONCERT TICKET                 ||" << endl;
    cout << "||                                                    ||" << endl;
    cout << "========================================================" << endl;
    cout << setw(12) << left << "Seats" << ": " << Ticket[ticketIndex].seat;
    cout << setw(12) << left << "\nDate" << ": " << "5th December 2023" << endl;
    cout << setw(12) << left << "Time" << ": " << "8:00 to 10:00pm" << endl;
    cout << setw(12) << left << "Venue" << ": " << "ABC Concert Hall" << endl << endl;
    cout << setw(12) << left << "Serial Number" << ": " << Ticket[ticketIndex].serialNum << endl;
    cout << "**Serial number is used for rechecking purposes.**" << endl << endl;
    cout << setw(49) << right << "Total payment: RM" << fixed << setprecision(2) << Ticket[ticketIndex].price << endl;
    cout << "========================================================" << endl << endl << endl;
}


//Function for checking ticket via serial number or via login
void check_serialNum(ticketType Ticket[])
{
    system("cls");
    int numCheck;
    bool foundNum = false;

    cout << "Enter your ticket serial number (e.g. 87654321): ";
    cin >> numCheck;
    for (int i = 0; i < 280; i++)
    {
        if (numCheck == Ticket[i].serialNum)
        {
            displayTicket(Ticket, i);
            foundNum = true;
            break;
        }
    }
    if (foundNum == false)
        cout << "Your serial number is invalid. Please try again." << endl;


}
void check_user(userType User[], ticketType Ticket[])
{
    system("cls");
    cin.get();
    string input_username, input_password;
    cout << "Enter username: ";
    getline(cin, input_username);
    cout << "Enter password: ";
    getline(cin, input_password);

    for (int i = 0; i < 280; i++)//to validate which user account
    {
        if (input_username == User[i].username && input_password == User[i].password)
        {
            for (int j = 0; j < 280; j++)//j is every element stored in ticket_bought
            {
                for (int q = 0; q < 280; q++)//q is index of ticket
                {
                    if (User[i].ticket_bought[j] == Ticket[q].serialNum)
                    {
                        if (User[i].ticket_bought[j] == 0)//to make the code more efficient
                            break;

                        displayTicket(Ticket, q);
                        break;
                    }


                }

            }
            break;
        }
    }
}
void checkTicket(userType User[], ticketType Ticket[])
{
    char choice;
    do {
        system("cls");
        cout << "a) Check ticket by serial number" << endl;
        cout << "b) Check ticket by login" << endl;
        cout << "c) Exit" << endl;
        cout << "Select(e.g. a) >> ";
        cin >> choice;
        if (isupper(choice) != 0)
        {
            choice = static_cast<char>(tolower(choice));
        }
        switch (choice)
        {
        case 'a':
            check_serialNum(Ticket);
            cout << "Press enter to continue......" << endl;
            cin.ignore();
            cin.get();
            break;
        case 'b':
            check_user(User, Ticket);
            cout << "Press enter to continue......" << endl;
            cin.get();
            break;
        case 'c':
            break;
        default:
            cout << "Invalid selection. Please try again." << endl;
            cout << "Press enter to continue......" << endl;
            cin.ignore();
            cin.get();
        }
    } while (choice != 'c');

}

//promt user to give feed back
void getFeedback(string feedback[], int& feedbackCount)
{
    system("cls");
    cin.get();
    cout << "Describe your feedback: " << endl;
    getline(cin, feedback[feedbackCount]);
    feedbackCount++;
    cout << "\nThanks for your feedback." << endl;
}
//printing out feedback from user in admin panel
void printFeedback(string feedback[], int& feedbackCount)
{
    system("cls");
    cout << "Received feedback: " << endl;
    for (int i = 0; i < feedbackCount; i++)
    {
        int index = i;
        cout << setw(2) << left << index + 1 << ") " << feedback[i] << endl;
    }
    cout << "Press enter to continue......" << endl;
    cin.ignore();
    cin.get();
}


//admin panel
// Function for admin login
bool AdminLogin() {
    system("cls");
    string username, password;
    cout << "Enter Admin Username: ";
    cin.ignore();
    getline(cin, username);
    cout << "Enter Admin Password: ";
    getline(cin, password);

    // Replace the following hardcoded values with the actual staff username and password
    return (username == "Test1234" && password == "password");
}
// Function to display staff menu
void displayStaffMenu() {
    cout << "======= Staff Menu =======" << endl;
    cout << "1. Sales Report" << endl;
    cout << "2. Feedback From User" << endl;
    cout << "3. Suspend Ticketing" << endl;
    cout << "4. Reactivate Ticketing" << endl;
    cout << "5. Cancel Concert" << endl;
    cout << "6. Exit Staff Menu" << endl;
}
// Function to cancel the concert
bool CancelConcert() {
    char Response;
    bool concertCancelled = false;
    string SecretCode;
    cout << "Enter 8 digit code: ";
    cin >> SecretCode;
    if (SecretCode == "01234567") {
        cout << "Are you sure you want to cancel the concert? (Y/N): ";
        cin >> Response;
        if (Response == 'y' || Response == 'Y') {
            cout << "Concert has been canceled. Exiting program..." << endl;
            concertCancelled = true;
            Sleep(2000);
            exit(0);
        }
        else {
            cout << "Concert cancellation aborted." << endl;
        }
    }
    else {
        cout << "Incorrect Code\n";
    }

    return concertCancelled;
}
// Function to suspend ticketing ordering system
bool suspendTicketing() {
    string susTickCode;
    char susTickResponse;
    bool ticketSuspend = false;

    cout << "Enter the 6 digit code: " << endl;
    cin >> susTickCode;
    if (susTickCode == "123456") {
        cout << "Are you sure you want to suspend the ticketing process? (y/n): ";
        cin >> susTickResponse;
        if (susTickResponse == 'y' || susTickResponse == 'Y') {
            cout << "Ticket ordering system will be suspended." << endl;
            ticketSuspend = true;
        }
        else {
            cout << "Ticket suspention aborted." << endl;
        }
    }
    else {
        cout << "Incorrect Code\n";
    }
    cout << "\nPress enter to continue......" << endl;
    cin.ignore();
    cin.get();
    return ticketSuspend;
}
// Function to reactivate ticketing ordering system
bool reactivateTicketing() {
    string susTickCode;
    char susTickResponse;
    bool ticketSuspend = true;

    cout << "Enter the 6 digit code: " << endl;
    cin >> susTickCode;
    if (susTickCode == "123456") {
        cout << "Are you sure you want to reactivate the ticketing process? (y/n): ";
        cin >> susTickResponse;
        if (susTickResponse == 'y' || susTickResponse == 'Y') {
            cout << "Ticket ordering system will be reactivated." << endl;
            ticketSuspend = false;
        }
        else {
            cout << "Ticket reactivation aborted." << endl;
        }
    }
    else {
        cout << "Incorrect Code\n";
    }
    cout << "\nPress enter to continue......" << endl;
    cin.ignore();
    cin.get();
    return ticketSuspend;
}
//function for sales report
void salesReport(int total_seatCount, double total_revenue, int totalVipSeat, int totalExcSeat, int totalRegSeat) {
    system("cls");
    cout << "=== Sales Report ===\n";
    cout << setw(20) << left << "Ticket Category" << setw(20) << "Quantity Sold" << setw(20) << "Revenue" << endl;
    cout << setw(20) << left << "VIP" << setw(20) << totalVipSeat << setw(20) << totalVipSeat * VIP_PRICE << endl;
    cout << setw(20) << left << "Executive" << setw(20) << totalExcSeat << setw(20) << totalExcSeat * EXC_PRICE << endl;
    cout << setw(20) << left << "Regular" << setw(20) << totalRegSeat << setw(20) << totalRegSeat * REG_PRICE << endl;
    cout << "=====================================================" << endl;
    cout << setw(20) << left << "Total seats sold" << ": " << total_seatCount << endl;
    cout << setw(20) << left << "Unsold seats" << ": " << 280 - total_seatCount << endl;
    cout << setw(20) << left << "Total revenue" << ": RM" << fixed << setprecision(2) << total_revenue << endl;
    cout << setw(20) << left << "Total profit" << ": RM" << fixed << setprecision(2) << total_revenue * 0.7 << endl;
    cout << "Press enter to continue......" << endl;
    cin.ignore();
    cin.get();
}
//function to access staff menu
void staffMenu(bool& ticketSuspend, bool& concertCancelled, string feedback[], int& feedbackCount, int total_seatCount, double total_revenue, int totalVipSeat, int totalExcSeat, int totalRegSeat) {
    int SMchoice;

    do
    {
        system("cls");
        displayStaffMenu();
        cout << "Enter your choice (1-5): ";
        cin >> SMchoice;

        switch (SMchoice) {
        case 1:
            salesReport(total_seatCount, total_revenue, totalVipSeat, totalExcSeat, totalRegSeat);
            break;
        case 2:
            printFeedback(feedback, feedbackCount);
            break;
        case 3:
            ticketSuspend = suspendTicketing();
            break;
        case 4:
            ticketSuspend = reactivateTicketing();
            break;
        case 5:
            concertCancelled = CancelConcert();
            break;
        case 6:
            cout << "Exiting Staff Menu.\n";
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 6.\n";
        }
    } while (SMchoice != 6);
}

