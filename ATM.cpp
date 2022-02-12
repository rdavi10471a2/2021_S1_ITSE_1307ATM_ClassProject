#include "ATM.h"
#include "Account.h"
#include "Transaction.h"

//Initialize class static variables here these are incrimented by 1 each time an account is created
//see getNextAccount() and getNextPin()
int ATM::GlobalAccountNumber = 11111; //starting account number incrimented by one each time account is created
int ATM::GlobalPinNumber = 11111;  //starting pin number incrimented by one each time account is created

//creates 10 acccounts
//simulates ATM having acccess to bank account information
void ATM::CreateAccounts() 
{
	for (int i = 0; i < 10; i++)
	{
		//add account to vector by calling Account constructor
		this->CustomerAccounts.push_back(Account("FirstName_" + std::to_string(i) , "LastName_" + std::to_string(i), this->getNextAccount(), this->getNextPIN(), 0));
	}
}


//used internally to find accounts by pin and account number 
//suports login functionality
//Sets currentUser when a user is found in the CustomerAccounts Vector
void ATM::FindAccountByNumberAndPin(std::string acct, std::string pin)
{
	
	Account Dummy("", "", acct, pin, 0);  //create dummy object to use with std::find

	std::vector<Account>::iterator it;  //iterator

	//search by account and PIN
	it = std::find(CustomerAccounts.begin(), CustomerAccounts.end(), Dummy);  //uses overloaded == operator

	//standard way to test if find found something
	if (it != CustomerAccounts.end()) {
		currentUser = it._Ptr; //pointer to element in vector make current user point to this
	}
	else
		currentUser = nullptr;
	
}
//read from console to get user info;

 void  ATM::GetUserDetais() {
	string acct;
	string pin;
	std::cout << endl << "Please enter your account number" << endl;
	std::cin >> acct;
	std::cout << "Please Enter Your Pin"<<endl;
	cin >> pin;
	if(currentUser!= nullptr) //if we have a user make it look like we dont so we can get another one (and current one doest stay if login attempt fails)
	   currentUser = nullptr;  //reset to null so we loose connection to user we previously had and we can detect not having found user
	FindAccountByNumberAndPin(acct, pin); //search Vector Accuonts for login info supplied
}

 //main execution method for the ATM 
 // allows users to *login* and create /view transactions
void ATM::RunSystem() {

	int userchoice=0;
	int logintries = 0;
	
	//Allow user 3 login attempts
	
	while (logintries < 3)
	{
		GetUserDetais();//sets ATM.currentUser pointer to logged in user if login is successful
		
		//we found a user that matches account and pin number
		if ( currentUser!=NULL) {
			break;
		}
		logintries++;
	}
	
	//we have a user so we can work with it
	if (currentUser!=NULL)
	{
		vector<Transaction> trans;  //current user transactions when we need them below

		do
		{
			PrintMainMenu(); //print the menu
			float transAmt = 0; //temp variable transaction amount so we can check to make sure it is positive before creating transaction
			userchoice = getMenuChoiceIntRange(0, 5);

			switch (userchoice)
			{
				case 1: //Deposit
					cout << endl << "Enter Deposit Amount" << endl;
					cin >> transAmt;
					if (transAmt > 0)
					{
						currentUser->MakeDepoist(transAmt);
					}
					else
					{
						std::cout << endl << "Depost must be positive";
					}
					break;
				case 2: //withdrawl
					cout << endl << "Enter Widthdrawl Amount" << endl;
					cin >> transAmt;
					if (transAmt > 0)
					{
                     currentUser->MakeWithdrawl(transAmt);
					}
					else
					{
						std::cout << endl << "Withdrawl  must be positive";
					}
					break;
				case 3: //display User with balance note dereference of pointer so we can use the overloaded operator <<
					cout << *currentUser;
					break;
				case 4: //display balance and trasactions
					cout << endl << "--------------------Current User Balance------------" << endl  ;
					cout << *currentUser;
					trans = currentUser->GetTransactions();
					cout << endl <<"Transactions below blank if no transactions are available" << endl;
					for (int i = 0; i < trans.size(); i++)
						cout << trans[i];
					cout << endl <<endl << "--------------------End User Balance------------" << endl;
					break;
				case 5: //Change user
					logintries = 0;
					while (logintries < 3)
					{
						GetUserDetais();

						if (currentUser != NULL) {
							break;
						}
						logintries++;
					}
					if (logintries == 3) {
						userchoice = 0;
						cout << endl << " Maximum number of login attemps exhausted";
					}
					break;
				case 0:
					break;
			
			}
		} while (userchoice != 0);
	}
	else {
	std:cout << endl << " Maximum number of login attemps exhausted";
	}
};

void ATM::PrintMainMenu() 
{
	cout << endl;
	cout << "----------------------------" << endl;
	cout << "|.......ATM MENU...........|" << endl;
	cout << "|1) Make Deposit...........|" << endl;
	cout << "|2) Make Withdrawl.........|" <<endl;
	cout << "|3) Show Balance...........|" << endl;
	cout << "|4) Display Transactions...|" << endl;
	cout << "|5) Change User............|" << endl;
	cout << "|0) Quit the ATM...........|" << endl;
	cout << "|..........................|" << endl;
	cout << "----------------------------" << endl;
	cout << "Please choose an option :" << endl;
}


//ignore all input in stream up to newline after reading desired data
void ignoreLine()
{
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
//borrowed and modified from https://www.learncpp.com/cpp-tutorial/stdcin-and-handling-invalid-input/
//note this does not check for the meaning of the integer to the caller whether it is in the expected range etc just that the value entered was an integer
//accept values between minimum and 
int  ATM::getMenuChoiceIntRange(int low, int high)
{
	bool accept = false;
	int retvalue = -1;
	while (!accept) // Loop until user enters a valid input
	{

		int InputValue{};
		cin >> InputValue;

		// Check for failed extraction
		if (cin.fail()) // has a previous extraction failed?
		{
			// yep, so let's handle the failure
			cin.clear(); // put us back in 'normal' operation mode
			ignoreLine(); // and remove the bad input
			cout << "Oops, that input is invalid.  Please try again.\n"; //error output stream see https://www.cplusplus.com/reference/iostream/cerr/
			accept = false;
			PrintMainMenu();
		}
		else
		{
			ignoreLine(); // remove any extraneous input
			// the user can't enter a meaningless integer value, so we don't need to worry about validating that
			if (InputValue >= low && InputValue <= high) {
				retvalue = InputValue;
				accept = true;
			}
			else
			{
				std::cout << "Invalid Choice Please Retry" << endl;
				PrintMainMenu();
			}
		}
		
	}
	return retvalue;
}