#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <cctype>
#include <conio.h>
#include <ctime>
#include <fstream>
#include <stdio.h>
#include <dirent.h>

using namespace std;

void bookingForm(int& bookingCase);
void currentBookings();
void viewDetailedInfo();
void bookingRate();
void viewReport();
void advSettings();
void verifyStudent(bool& studentCard);
void bookingMenu();
void endPrompt();
void waitEsc();
void writeData(bool targetAll, string target, float value);

int pressedKey;
int bookingNum;
bool walkInIsOpen = true;
string strWalkInTime;
float revenueOfDay[7], netRevenue, totalRevenue;
int  timesArray[25] = { 8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 
	                    18, 19, 20, 21, 22, 23, 24,  1,  2
};             

struct storeData {
	int bookingCase;
	string referenceID;
	
	string nameOfTheDay, bookingDate;
	int dayOfTheWeek;
	int numInTimesArray, timesArray[25];
	int timeIn, timeOut;
	string strTimeIn, strTimeOut; 
	string antePostIn, antePostOut;
	int duration, maxDuration;
	int courtAmount;
	string selectedCourtList;
	
	string customerName, customerIC; 
	bool studentCard;
};
struct receiptData {
	float rate[6];
	float oneCourtPrice, netPrice, subTotal, total;
	float deposit;
	float discount;
	float SST;
	float roundAdj;
	float cash, change;
	int dayAmount, nightAmount, midnightAmount;
	float dayPrice, nightPrice, midnightPrice;
	bool firstHour, secondHour, thirdHour;
	int extraHourAmount;
	float extraHourPrice;
};

string systemDate() {
  	string day, month, year, systemDate;
    time_t t = time(0);
    tm* now  = localtime(&t);
	stringstream ss_Date;
	
    ss_Date << (now -> tm_mday) << ' ' << (now -> tm_mon + 1) << ' '
            << (now -> tm_year + 1900);	
    ss_Date >> day >> month >> year;	
    if ((now -> tm_mday) < 10)
        day   = '0' + day;
	if ((now -> tm_mon + 1) < 9)
		month = '0' + month; 
	systemDate = day + '/' + month + '/' + year;
	return systemDate;
}
string systemTime() {
	string sec, min, hour, systemTime;
    time_t t = time(0);
    tm* now  = localtime(&t);
	stringstream ss_Time;
	
    ss_Time << (now -> tm_hour) << ' ' << (now -> tm_min) << ' ' 
            << (now -> tm_sec);	
    ss_Time >> hour >> min >> sec;
    if ((now -> tm_sec) < 10)
        sec  = '0' + sec;
	if ((now -> tm_min) < 10)
		min  = '0' + min; 
	if ((now -> tm_hour) < 10)
		hour = '0' + hour; 
	systemTime = hour + ':' + min + ':' + sec;
	return systemTime;
}
string walkInTime() {
	int hour;
	string antePostIn;
	stringstream ss_walkInTime;
	stringstream(systemTime().substr(0,2)) >> hour;
	hour += 1;
	
	if (hour > 0 && hour < 8) {
		walkInIsOpen = false;
		return " CLOSED ";
	}
	else {
		if (hour < 12 || hour > 23) {
			if (hour == 24)
				hour -= 12;
			antePostIn = " AM";
		} else {
			if (hour != 12)
				hour -= 12;
			antePostIn = " PM";
		}		                               
		ss_walkInTime << setfill('0') << setw(2) << hour << ":00";
		ss_walkInTime >> strWalkInTime;
		return strWalkInTime + antePostIn;
	}
}
int dayOfWeek(int bookingCase, int day, int month, int year) {
	if (bookingCase == 1) {
		time_t t = time(0);
    	tm* now  = localtime(&t);
		return now -> tm_wday;
	}
	else {
		tm time_in = {0, 0, 0, day, month - 1, year - 1900};
		time_t time_temp = mktime(&time_in);
    	tm * time_out = localtime(&time_temp);
		return time_out -> tm_wday;
	}
}

int slider(
	int& pressedKey, 
	int value, 
	int min, 
	int max, 
	int overwrite
	) {
		pressedKey = getch();
		if (pressedKey == 0 || pressedKey == 224) {
			pressedKey = getch();
			if (pressedKey == 77 || pressedKey == 72) {      // Right/Up
				value++;
				if (value > max)
					value -= 1;
			}
			else if (pressedKey == 75 || pressedKey == 80) { // Left/Down
				value--;
				if (value < min)
					value += 1;	
			}
			for (int r = 0; r < overwrite; r++)              // Overwriting previous characters
				cout << "\b";
			return value;
		}
		else if (pressedKey == 13)
			return value;
		else
			slider(pressedKey, value, min, max, overwrite);
}

void itemDetails(receiptData& print, storeData& save) {
	cout << "   1 Court Reservation"                     << endl;
	cout << "      ID#(" << save.referenceID << ")"      << endl;
	cout << "      " << save.nameOfTheDay << ' ' << save.bookingDate << endl;
	cout << "      " << save.strTimeIn << '-' << save.strTimeOut << endl;
	if (print.dayAmount != 0)
		cout << "      Day Rate         " << setfill(' ') << setw(2) << print.dayAmount << " * " 
										  << setprecision(2) << fixed << print.rate[0] << "  " 
										  << setfill(' ') << setw(6) << setprecision(2) << fixed 
										  << print.dayPrice << endl;
	if (print.nightAmount != 0)
		cout << "      Night Rate       " << setfill(' ') << setw(2) << print.nightAmount << " * " 
										  << setprecision(2) << fixed << print.rate[2] << "  " 
										  << setfill(' ') << setw(6) << setprecision(2) << fixed 
										  << print.nightPrice << endl;
	if (print.midnightAmount != 0)
		cout << "      Midnight Rate    " << setfill(' ') << setw(2) << print.midnightAmount << " * " 
										  << setprecision(2) << fixed << print.rate[1] << "  " 
										  << setfill(' ') << setw(6) << setprecision(2) << fixed 
										  << print.midnightPrice << endl;
	if (print.firstHour == true)
		cout << "      1 Hour " << setfill(' ') << setw(28) << setprecision(2) << fixed 
								<< print.rate[3] << endl;
	if (print.secondHour == true)
		cout << "      2 Hours" << setfill(' ') << setw(28) << setprecision(2) << fixed 
								<< print.rate[4] << endl;
	if (print.thirdHour == true)
		cout << "      3 Hours" << setfill(' ') << setw(28) << setprecision(2) << fixed 
								<< print.rate[5] << endl;
	if (print.extraHourAmount != 0)
		cout << "      Extra Hours" << setfill(' ') << setw(8) << print.extraHourAmount << " * " 
									<< setprecision(2) << fixed	<< print.rate[0] << "  " 
									<< setfill(' ') << setw(6) << setprecision(2) << fixed 
									<< print.extraHourPrice << endl;
	cout << "                             ------------ " << endl;
	cout << "      Court: " << save.selectedCourtList << " " << save.courtAmount << " *       " 
							<< setfill(' ') << setw(7) << setprecision(2) << fixed 
							<< print.oneCourtPrice << endl;
	cout << "                             ------------ " << endl;
	cout << "     ITEM PRICE" << setfill(' ') << setw(26) << setprecision(2) << fixed 
							  << print.netPrice << endl;
	cout << " -----------------------------------------" << endl;	
}
void printReceipt(receiptData& print, storeData& save) {
	cout << "             FORUM 19 SDN BHD             " << endl;
	cout << "         COMPANY REGIS: XXXXXXX-X         " << endl;
	cout << "       LOT 1, JALAN 19/1B, SEKSYEN        " << endl;
	cout << "         19, 46300 PETALING JAYA,         " << endl;
	cout << "            SELANGOR, MALAYSIA            " << endl;
	cout << "             TEL : XX-XXXXXXXX            " << endl;
	cout << endl;
	cout << "                Tax Invoice               " << endl;
	cout << endl;
	cout << "  Booking#: " << setfill('0') << setw(3) << bookingNum   << endl;
	cout << "  Date    : " << systemDate() << ' ' << systemTime() << endl;
	cout << " -----------------------------------------" << endl;
	cout << "  NO ITEM             QTY    RATE   PRICE " << endl;
	itemDetails(print, save);
	cout << "  Net Price:"              << setfill(' ')  << setw(29) << setprecision(2) << fixed 
										<< print.netPrice << endl;
	cout << "  Student Discount (20%):" << setfill(' ')  << setw(16) << setprecision(2) << fixed 
										<< print.discount << endl;
	cout << "  Booking Deposit:"        << setfill(' ')  << setw(23) << setprecision(2) << fixed 
										<< print.deposit  << endl;
	cout << " -----------------------------------------" << endl;
	cout << "  SubTotal:"               << setfill(' ')  << setw(30) << setprecision(2) << fixed 
										<< print.subTotal << endl;
	cout << "  Service Tax (6%):"       << setfill(' ')  << setw(22) << setprecision(2) << fixed 
										<< print.SST      << endl;
	cout << "  Rounding Adj:"           << setfill(' ')  << setw(26) << print.roundAdj  << endl;
	cout << "                             -------------" << endl;
	cout << "  TOTAL" <<setw(27)<< "RM "<< setfill(' ')  << setw(7)  << setprecision(2) << fixed 
										<< print.total    << endl;
	cout << " -----------------------------------------" << endl;
	cout << "  Cash:"                   << setfill(' ')  << setw(34) << setprecision(2) << fixed 
										<< print.cash     << endl;
	cout << "  Change:"                 << setfill(' ')  << setw(32) << setprecision(2) << fixed 
										<< print.change   << endl;
	cout << endl;
	cout << "   Please keep this receipt as prove of   " << endl; 
	cout << "    payment and to redeem your deposit.   " << endl;
	cout << " *****************************************" << endl;
	cout << "      THANK YOU AND PLEASE COME AGAIN!    " << endl;
	cout << endl;
	cout << " [ESC] BACK TO BOOKING MENU" << endl;
	cout << endl;
	waitEsc();
	bookingMenu();
}
void getPayment(receiptData& print) {
	float cash, change;
	cout << " ---------------------------"   << endl;
	cout << "  Net Price              RM : " << setprecision(2) << fixed << print.netPrice << endl;
	cout << "  Student Discount (20%) RM : " << setprecision(2) << fixed << print.discount << endl;
	cout << "  Booking Deposit        RM : " << setprecision(2) << fixed << print.deposit  << endl;
	cout << " ---------------------------"   << endl;
	cout << "  SubTotal               RM : " << setprecision(2) << fixed << print.subTotal << endl;
	cout << "  Service Tax (6%)       RM : " << setprecision(2) << fixed << print.SST      << endl;
	cout << "  Rounding Adj           RM : " << print.roundAdj  << endl;
	cout << " ---------------------------"   << endl;
	cout << "  TOTAL                  RM : " << setprecision(2) << fixed << print.total    << endl;
	cout << " ---------------------------"   << endl;
	do {
		cout << "  Cash Payment           RM : ";
		cin >> cash;
		if (cin.fail() || cash < print.total) {
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "\n [!] Invalid value input!\n" << endl;
		}
	} while (cin.fail() || cash < print.total);
		
	change = cash - print.total;
	cout << "  Change                 RM : " << setprecision(2) << fixed << change << endl;
	cout << " ---------------------------"   << endl;
	cout << endl;	
	print.cash = cash;
	print.change = change;
	
	netRevenue += (print.subTotal - print.deposit);
	totalRevenue += (print.total - print.deposit);
	
	writeData(false, "writeNetRevenue", netRevenue);
	writeData(false, "writeTotalRevenue", totalRevenue);
}
void calcPrice(receiptData& print, storeData& save) {
		print.oneCourtPrice = 0;
		print.dayAmount = 0;
		print.nightAmount = 0;
		print.midnightAmount = 0;
		print.firstHour = false;
		print.secondHour = false;
		print.thirdHour = false;
		print.extraHourAmount = 0;
		print.deposit = 10.00;
		print.discount = -0.20;
		print.SST = 0.06;
		float rate[6] = {10, 15, 20, 25, 45, 60};
		print.rate[0] = 10, print.rate[1] = 15, print.rate[2] = 20;
		print.rate[3] = 25,	print.rate[4] = 45,	print.rate[5] = 60;
		string nameOfTheDay[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
		
		int duration = save.duration;
		int numInTimesArray = save.numInTimesArray;
		if (save.dayOfTheWeek >= 1 && save.dayOfTheWeek <= 5) { // Mon-Fri
			while (numInTimesArray < 10 && duration > 0) { 
				print.oneCourtPrice += rate[0];         // 8AM-6PM
				print.dayPrice += rate[0];
				print.dayAmount++;
				duration--;
				numInTimesArray++;
			}
			while (numInTimesArray < 16 && duration > 0) { 
				print.oneCourtPrice += rate[2];         // 6PM-12AM
				print.nightPrice += rate[2];
				print.nightAmount++; 
				duration--;
				numInTimesArray++;
			}
			while (numInTimesArray < 18 && duration > 0) { 
				print.oneCourtPrice += rate[1];         // 12AM-2AM
				print.midnightPrice += rate[1];
				print.midnightAmount++; 
				duration--;
				numInTimesArray++;
			}
		}
		else {
			if (duration > 2) {
				print.oneCourtPrice = rate[5];          // 3 Hours
				print.thirdHour = true;
				while (duration > 3) {
					print.oneCourtPrice += rate[0];     // >3 Hours
					print.extraHourPrice += rate[0];
					print.extraHourAmount++;
					duration--;
				}
			} 
			else if (duration == 2) {
				print.oneCourtPrice = rate[4];          // 2 Hours
				print.secondHour = true;
			}
			else {
				print.oneCourtPrice = rate[3];          // 1 Hour
				print.firstHour = true;
			}
		}
		print.netPrice = print.oneCourtPrice * save.courtAmount;
		print.subTotal = print.netPrice;
		if (save.studentCard == true) {
			print.discount *= print.subTotal;
			print.subTotal += print.discount;
		}
		else
			print.discount = 0.00; 
		
		if (save.bookingCase == 2)
			print.subTotal += print.deposit;
		else
			print.deposit = 0;
		print.SST *= print.subTotal;
		print.total = print.subTotal + print.SST;
		
    	float whole = floor(print.total*10);
    	float decimal = (print.total * 10) - whole;
    	decimal /= 10;
    	
		print.roundAdj = round(decimal/0.05) * 0.05; 
		print.roundAdj -= decimal; 
		print.total += print.roundAdj;
		
		save.nameOfTheDay = nameOfTheDay[save.dayOfTheWeek];
		revenueOfDay[save.dayOfTheWeek] += (print.subTotal - print.deposit);
		writeData(false, "write" + nameOfTheDay[save.dayOfTheWeek] + "Revenue", 
				  revenueOfDay[save.dayOfTheWeek]);
		
		getPayment(print);
}

string setBookingDate(storeData& save) {
	int DAY, MONTH, YEAR, currentYear;
	stringstream ss_bookingDate;
	stringstream(systemDate().substr(0,2)) >> DAY;
	stringstream(systemDate().substr(3,5)) >> MONTH;
	stringstream(systemDate().substr(6,9)) >> YEAR;
	currentYear = YEAR;
	cout << "                           DAY               MONTH               YEAR" << endl;
	cout << " Date (DD/MM/YYYY) : ";
	do {
		cout << "[-]    " << setfill('0') << setw(2) << DAY << "    [+] ";
		DAY = slider(pressedKey, DAY, 1, 31, 17);
	} while (pressedKey != 13);
	do {
		cout << "/ [-]    " << setfill('0') << setw(2) << MONTH << "    [+] ";
		MONTH = slider(pressedKey, MONTH, 1, 12, 19);
	} while (pressedKey != 13);
	do {
		cout << "/ [-]   " << YEAR << "   [+] ";
		YEAR = slider(pressedKey, YEAR, currentYear, 2030, 19);
	} while (pressedKey != 13);
	cout << endl;
	ss_bookingDate << setfill('0') << setw(2) << DAY << '/' 
	               << setfill('0') << setw(2) << MONTH << '/' 
				   << YEAR;
	ss_bookingDate >> save.bookingDate;
	save.dayOfTheWeek = dayOfWeek(2, DAY, MONTH, YEAR);
	
	return save.bookingDate;
}
void setDateAndTime(int& bookingCase, storeData& save) {
	if (bookingCase == 1) {                                 
		cout << " Date (DD/MM/YYYY) : " << systemDate() << endl;
		save.bookingDate = systemDate();
		save.dayOfTheWeek = dayOfWeek(1, 0, 0 ,0);
		int hour;
		stringstream(systemTime().substr(0,2)) >> hour;
		hour += 1;
		save.timeIn = hour;
		if (hour < 12 || hour > 23) 
			save.antePostIn = "AM";	
		else
			save.antePostIn = "PM";
		cout << " Start Time        : " << strWalkInTime << ' ' << save.antePostIn << endl;        
	}
	else {
		save.bookingDate = setBookingDate(save);
		int numInTimesArray = save.numInTimesArray;
		cout << " Start Time        : ";
		do {
			save.timeIn = timesArray[numInTimesArray];
			save.maxDuration = 18 - numInTimesArray;
			if (numInTimesArray < 4 || numInTimesArray > 15 || numInTimesArray == 17) {
				if (numInTimesArray == 16)
					save.timeIn -= 12;
				save.antePostIn = "AM";
			}
			else {
				if (numInTimesArray != 4)
					save.timeIn -= 12;
				save.antePostIn = "PM";
			}
			cout << 
			"[-] " << setfill('0') << setw(2) << save.timeIn << ":00 "<< save.antePostIn << " [+]";
			numInTimesArray = slider(pressedKey, numInTimesArray, 0, 17, 16);		
		} while (pressedKey != 13);
		cout << endl;
		save.numInTimesArray = numInTimesArray;                       
	}
}
void setDuration(int& bookingCase, storeData& save) {
	if (bookingCase == 1) {
		if (save.timeIn == 1)
			save.maxDuration = 1;
		else if (save.timeIn == 24)
			save.maxDuration = 2;
		else
			save.maxDuration = 3;
		
		if (save.timeIn < 8)
			save.numInTimesArray = save.timeIn + 16;
		else	
			save.numInTimesArray = save.timeIn - 8;
		if (save.timeIn > 12)
			save.timeIn -= 12;
	}	
	save.duration = 1;
	cout << " Duration (hours)  : ";
	do {
		cout << "[-]    " << setfill(' ') << setw(2) << save.duration << "    [+]";
		save.duration = slider(pressedKey, save.duration, 1, save.maxDuration, 16);		
	} while (pressedKey != 13);
	cout << endl;
	int tempNumInTimesArray = save.numInTimesArray + save.duration;
	save.timeOut = timesArray[tempNumInTimesArray];
	if (tempNumInTimesArray < 4 || tempNumInTimesArray > 15 || tempNumInTimesArray == 17) {
		if (tempNumInTimesArray == 16)
			save.timeOut -= 12;
		save.antePostOut = "AM";
	}
	else {
		if (tempNumInTimesArray != 4)
			save.timeOut -= 12;
		save.antePostOut = "PM";
	}
}
int setCourtAmount(int courtAmount) {
	cout << " Number of Courts  : ";
	do {
		cout << "[-]    " << setfill(' ') << setw(2) << courtAmount << "    [+]";
		courtAmount = slider(pressedKey, courtAmount, 1, 5, 16);	
	} while (pressedKey != 13);
	cout << endl;
	
	return courtAmount;
} 
void courtSelection(storeData& save, int& courtAmount) {
	char courtCode[16] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                          'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'
	};
	int amount = 0;
	char selectedCourt[16];
	cout << " Select your court : ";
	for (int numInCodeArray = 0; amount < courtAmount; amount++) {
		do {
			cout << "[<]     " << courtCode[numInCodeArray] << "    [>]";
			numInCodeArray = slider(pressedKey, numInCodeArray, 0, 15, 16);	
		} while (pressedKey != 13);
		selectedCourt[amount] = courtCode[numInCodeArray];
		cout << endl;
		cout << "                     ";
	}
	cout << "\r Courts Selected   :";
	for (int numInCodeArray = 0; numInCodeArray < amount; numInCodeArray++) {
		cout << ' ' << selectedCourt[numInCodeArray];
		save.selectedCourtList += selectedCourt[numInCodeArray];
		save.selectedCourtList += ' ';
	}
	while (amount < 5) {
		save.selectedCourtList += "  ";
		amount++;
	}
	cout << '\n' << endl;
}
void customerDetails(
	 string& customerName, 
	 string& customerIC, 
	 bool& studentCard) {
		cout << " Enter your name   : ";
		getline(cin, customerName);
		cout << " Enter your IC     : ";
		getline(cin, customerIC);
		verifyStudent(studentCard);
		cout << endl;
}	
void verifyStudent(bool& studentCard) {
	string student = "No";
	cout << " Student Discount  : ";
	do {
		cout << "[<]   " << setfill(' ') << setw(3) << student << "    [>]";
		pressedKey = getch();
		if (pressedKey == 0 || pressedKey == 224) {
			pressedKey = getch();
			if (pressedKey == 77 || pressedKey == 72) {
					student = "Yes";
					studentCard = true;
			}
			else if (pressedKey == 75 || pressedKey == 80) {
					student = "No";
					studentCard = false;	
			}
		}
		for (int r = 0; r < 16; r++)
			cout << "\b";
	} while (pressedKey != 13);
	cout << endl;
}
void timeIntToString(storeData& save) {
		stringstream ss_TimeIn, ss_TimeOut;
		ss_TimeIn << setfill('0') << setw(2) << save.timeIn << ":00 ";
		ss_TimeIn >> save.strTimeIn;
		save.strTimeIn += save.antePostIn;
		ss_TimeOut << setfill('0') << setw(2) << save.timeOut << ":00 ";
		ss_TimeOut >> save.strTimeOut;
		save.strTimeOut += save.antePostOut;
}
void referenceID(storeData& save) {
	int DAY, MONTH, YEAR;
	stringstream ss_referenceID;
	stringstream(save.bookingDate.substr(0,2)) >> DAY;
	stringstream(save.bookingDate.substr(3,5)) >> MONTH;
	stringstream(save.bookingDate.substr(6,9)) >> YEAR;
	char bookCase;
	if (save.bookingCase == 1)
		bookCase = 'A';
	else	
		bookCase = 'B';
	
	ss_referenceID << YEAR << setfill('0') << setw(2) << MONTH;
	ss_referenceID << setfill('0') << setw(2) << DAY;
	ss_referenceID << setfill('0') << setw(3) << bookingNum << bookCase;
	ss_referenceID >> save.referenceID;
}
void customerData(storeData& save) {
	string strCourtAmount;
	stringstream ss_courtAmount;
	ss_courtAmount << setfill('0') << setw(2) << save.courtAmount;
	ss_courtAmount >> strCourtAmount;
	string student = "No";
	if (save.studentCard == 1)
		student = "Yes";
	referenceID(save);

	string bookingData = 
	  " | " + save.bookingDate  
	+ " | " + save.strTimeIn 
	+ " | " + save.strTimeOut 
	+ " | " + save.selectedCourtList
	+ "| "  + save.referenceID
	+ " | ";
	
	stringstream ss_fileTable;
	ss_fileTable << "Booking_" << setfill('0') << setw(3);
	ss_fileTable << bookingNum;
	string bookTable = ss_fileTable.str();
	
	ofstream tableFile(
	("../Sports Center Booking Management System/Booking Data/Table Data/" 
	+ bookTable + ".txt").c_str()
	);
	tableFile << bookingData;
	tableFile.close();
	
	string text =
	" Reference No.            : " + save.referenceID       + "\n"
	" Customer Name            : " + save.customerName      + "\n" 
	" Customer IC              : " + save.customerIC        + "\n" 
	" Booking Date (DD/MM/YYYY): " + save.nameOfTheDay + ' ' 
	                               + save.bookingDate       + "\n"
	" Time In                  : " + save.strTimeIn         + "\n"
	" Time Out                 : " + save.strTimeOut        + "\n"
	" ---------------------------\n"
    " Selected Court           : " + save.selectedCourtList;
	
	stringstream ss_fileData;
	ss_fileData << "Booking_" << setfill('0') << setw(3);
	ss_fileData << bookingNum;
	string fileData = ss_fileData.str();
	
	ofstream detailedFile(
	("../Sports Center Booking Management System/Booking Data/Detailed Data/" 
	+ fileData + ".txt").c_str()
	);
	detailedFile << text;
	detailedFile.close();
}

void readAllFiles() {
	ifstream readBookingNumber(
	"../Sports Center Booking Management System/Booking Data/BookingNumber.txt"
	);
	ifstream readNetRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/NetRevenue.txt"
	);
	ifstream readTotalRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/TotalRevenue.txt"
	);
	ifstream readSunRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/0_Sunday.txt"
	);
	ifstream readMonRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/1_Monday.txt"
	);
	ifstream readTueRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/2_Tuesday.txt"
	);
	ifstream readWedRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/3_Wednesday.txt"
	);
	ifstream readThuRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/4_Thursday.txt"
	);
	ifstream readFriRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/5_Friday.txt"
	);
	ifstream readSatRevenue(
	"../Sports Center Booking Management System/Booking Data/Revenue Data/6_Saturday.txt"
	);
	
	readBookingNumber >> bookingNum;
	readNetRevenue >> netRevenue;
	readTotalRevenue >> totalRevenue;
	readSunRevenue >> revenueOfDay[0];
	readMonRevenue >> revenueOfDay[1];
	readTueRevenue >> revenueOfDay[2];
	readWedRevenue >> revenueOfDay[3];
	readThuRevenue >> revenueOfDay[4];
	readFriRevenue >> revenueOfDay[5];
	readSatRevenue >> revenueOfDay[6];
	
	readBookingNumber.close();
	readNetRevenue.close();
	readTotalRevenue.close();
	readSunRevenue.close();
	readMonRevenue.close();
	readTueRevenue.close();
	readWedRevenue.close();
	readThuRevenue.close();
	readFriRevenue.close();
	readSatRevenue.close();
}

int main() {
	readAllFiles();
	cout << "  ______                           __  ___  "    << endl;
	cout << " |  ____|                         /_ |/ _ \\ "   << endl;
	cout << " | |__ ___  _ __ _   _ _ __ ___    | | (_) |"    << endl;
	cout << " |  __/ _ \\| '__| | | | '_ ` _ \\   | |\\__, |" << endl;
	cout << " | | | (_) | |  | |_| | | | | | |  | |  / / "    << endl;
	cout << " |_|  \\___/|_|   \\__,_|_| |_| |_|  |_| /_/  "  << endl;
	cout << "             -------------------            "    << endl;
	cout << "           BADMINTON SPORTS CENTER          "    << endl;
	cout << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " |                MAIN MENU                |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " | [1] CREATE BOOKINGS                     |" << endl;
	cout << " | [2] VIEW CURRENT BOOKINGS               |" << endl;
	cout << " | [3] VIEW BOOKING RATE                   |" << endl;
	cout << " | [4] REVENUE REPORT                      |" << endl;
	cout << " | [5] ADVANCED SETTINGS (CLEAR DATA)      |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " | [ESC] EXIT PROGRAM                      |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << endl;	
	do {
		pressedKey = getch();
		switch (pressedKey) {
			case '1':
				bookingMenu();
			case '2':
				currentBookings();
			case '3':
				bookingRate();
				cout << " | [ESC] BACK TO MAIN MENU                 |" << endl;
				cout << " +-----------------------------------------+" << endl;
				cout << endl;
				waitEsc();
				main();
			case '4':
				viewReport();				
			case '5':
				advSettings();
			default:
				continue;
		}
	} while (pressedKey != 27);
	endPrompt();
	exit(0);
}
void bookingMenu() {
	cout << " +-----------------------------------------+" << endl;
	cout << " |              BOOKING MENU               |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " | [1] WALK-IN RESERVATION (" << walkInTime() << ")      |" << endl;
	cout << " | [2] QUICK BOOKING                       |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " | [ESC] BACK TO MAIN MENU                 |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << endl;
	int bookingCase;
	do {
		pressedKey = getch();
		switch (pressedKey) {
			case '1':
				if (walkInIsOpen == true)
					bookingForm(bookingCase = 1);
				break;
			case '2':
				bookingForm(bookingCase = 2);
			default:
				continue;
		}
	} while (pressedKey != 27);
	main();
}
void bookingForm(int& bookingCase) {
	storeData save;
	receiptData print;
	
	string bookingDate; 
	string customerName; 
	string customerIC; 
	int courtAmount = 1; 
	bool studentCard;
	
	save.numInTimesArray = 0;
	save.antePostIn = "AM";
	save.antePostOut = "AM";
	
	bookingRate();
	cout << " | Use arrow keys to change the value.     |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " ---------------" << endl;
	cout << " Booking Details" << endl;
	cout << " ---------------" << endl; 
	setDateAndTime(bookingCase, save);
	setDuration(bookingCase, save);
	courtAmount = setCourtAmount(courtAmount);
	courtSelection(save, courtAmount);
	cout << " ----------------" << endl;
	cout << " Customer Details" << endl;
	cout << " ----------------" << endl;
	customerDetails(customerName, customerIC, studentCard);
	
	timeIntToString(save);
	save.bookingCase = bookingCase;
	save.customerName = customerName;
	save.customerIC = customerIC;
	save.courtAmount = courtAmount;
	save.studentCard = studentCard;
	calcPrice(print, save);
	
	writeData(false, "writeBookingNumber", ++bookingNum);
	
	customerData(save);  // Save Data
	
	cout << " [i] DATA SUCCESSFULLY SAVED!\n" << endl;
	cin.ignore(INT_MAX, '\n');
	cout << " +-----------------------------------------+" << endl;
	cout << " | [i]   Do you want to print receipt?     |" << endl;
	cout << " |                                         |" << endl;
	cout << " |             [Y] Yes  [N] No             |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << endl;
	do {
		pressedKey = getch();
		if (pressedKey == 13 || tolower(pressedKey) == 'y') 
			printReceipt(print, save);
	} while (pressedKey != 27 && tolower(pressedKey) != 'n');
	bookingMenu();
}
void currentBookings() {
	string readList;
	cout << " +-----------------------------------------------------------------+" << endl;
	cout << " |                           ALL BOOKINGS                          |" << endl;
	cout << " +-----------------------------------------------------------------+" << endl;
	cout << " | NO. | DATE       | IN      | OUT     | COURT     | REFERENCE ID |" << endl;
	cout << " +-----------------------------------------------------------------+" << endl;
	if (bookingNum == 0)
		cout << 
		" |                 No Bookings Currently Available.                |" << endl;	
	for (int listNo = 1; listNo < (bookingNum + 1); listNo++) {
		stringstream ss_listNo;
		ss_listNo << setfill('0') << setw(3);
		ss_listNo << listNo;
		string listNumber = ss_listNo.str();
		
		ifstream MyReadFile(
		("../Sports Center Booking Management System/Booking Data/Table Data/Booking_" 
		+ listNumber + ".txt").c_str()
		);
		getline(MyReadFile, readList);					
		cout << " | " << setfill(' ') << setw(3) << listNo << readList << endl;
		MyReadFile.close();
	}
	cout << " +-----------------------------------------------------------------+" << endl;
	if (bookingNum != 0) {
		cout << " [ESC] BACK TO MAIN MENU | [ENTER] VIEW DETAILED INFO\n" << endl;
		viewDetailedInfo();
		main();
	}
	cout << " [ESC] BACK TO MAIN MENU\n" << endl;
	waitEsc();
	main();
}
void viewDetailedInfo() {
	string readData;
	int viewNum;
	do {
		pressedKey = getch();
		if (pressedKey == 13) {
			cout << " Enter the booking number to view: ";
			cin >> viewNum;
			if (viewNum > 0 && viewNum < (bookingNum + 1)) {
				stringstream ss_viewNum;
				ss_viewNum << setfill('0') << setw(3) << viewNum;
				string strViewNum = ss_viewNum.str();

				ifstream MyReadFile(
				("../Sports Center Booking Management System/Booking Data/Detailed Data/Booking_" 
				  + strViewNum + ".txt").c_str()
				);
				cout << endl;
				while(getline(MyReadFile, readData))
					cout << readData << endl;
				MyReadFile.close();
			}
			else
				cout << "\n [i] NO DATA AVAILABLE." << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "\n [ESC] BACK TO TABLE\n" << endl;
			waitEsc();
			currentBookings();
		}
	} while (pressedKey != 27);
}
void bookingRate() {
	cout << " +-----------------------------------------+" << endl;
	cout << " |             BOOKING RATES               |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " | Mon-Fri                                 |" << endl;
	cout << " | Day Rate      ( 8AM -  6PM) : RM10/hr   |" << endl;
	cout << " | Night Rate    ( 6PM - 12AM) : RM20/hr   |" << endl;
	cout << " | Midnight Rate (12AM -  2AM) : RM15/hr   |" << endl;
	cout << " |                                         |" << endl;
	cout << " | Sat-Sun                                 |" << endl;
	cout << " | 08:00AM - 02:00AM                       |" << endl;
	cout << " |  1 hour  : RM25                         |" << endl;
	cout << " |  2 hours : RM45                         |" << endl;
	cout << " |  3 hours : RM60                         |" << endl;
	cout << " | >3 hours : RM60 + RM10/Extra hour       |" << endl;
	cout << " |                                         |" << endl;
	cout << " | DISCOUNT 20% OFF FOR STUDENTS           |" << endl;
	cout << " | Show student card upon payment.         |" << endl;
	cout << " |                                         |" << endl;
	cout << " | Rates are calculated per court.         |" << endl;
	cout << " | Maximum amount of 5 courts allowed per  |" << endl;
	cout << " | booking.                                |" << endl;
	cout << " |                                         |" << endl;
	cout << " | Maximum of 3 hours allowed for walk-in  |" << endl;
	cout << " | reservation.                            |" << endl;
	cout << " | Walk-in reservation starts on the next  |" << endl;
	cout << " | hour from current time.                 |" << endl;
	cout << " | (eg. Booked@ 10:21AM, Starts@ 11:00AM)  |" << endl;
	cout << " |                                         |" << endl;
	cout << " | Reservation closed after 12:00 AM       |" << endl;
	cout << " |                                         |" << endl;
	cout << " | RM10 Deposit required for non-walk-in   |" << endl;
	cout << " | reservations.                           |" << endl;
	cout << " |                                         |" << endl;
	cout << " | Price shown are excluding SST.          |" << endl;
	cout << " +-----------------------------------------+" << endl;
}
void viewReport() {
	cout << " +-----------------------------------------+" << endl;
	cout << " |                REPORT                   |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " | Sunday            :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << revenueOfDay[0] << " |" << endl;
	cout << " | Monday            :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << revenueOfDay[1] << " |" << endl;
	cout << " | Tuesday           :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << revenueOfDay[2] << " |" << endl;
	cout << " | Wednesday         :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << revenueOfDay[3] << " |" << endl;
	cout << " | Thursday          :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << revenueOfDay[4] << " |" << endl;
	cout << " | Friday            :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << revenueOfDay[5] << " |" << endl;
	cout << " | Saturday          :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << revenueOfDay[6] << " |" << endl;										 		
	cout << " | Total Net Revenue :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << netRevenue << " |" << endl;
	cout << " | Total (Incl. GST) :         RM " << setfill(' ') << setw(8) 
												 << setprecision(2) << fixed 
												 << totalRevenue << " |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " | [ESC] BACK TO MAIN MENU                 |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << endl;
	waitEsc();
	main();
}
void writeData(bool targetAll, string target, float value) {
	for (int i = 0; i <= 10; i++) {
		string filePath;
		if (target == "writeBookingNumber" || (targetAll == true && i == 1))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/BookingNumber.txt";
		
		if (target == "writeNetRevenue" || (targetAll == true && i == 2))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/NetRevenue.txt";
		if (target == "writeTotalRevenue" || (targetAll == true && i == 3))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/TotalRevenue.txt";
		
		if (target == "writeSunRevenue" || (targetAll == true && i == 4))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/0_Sunday.txt";
		if (target == "writeMonRevenue" || (targetAll == true && i == 5))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/1_Monday.txt";
		if (target == "writeTueRevenue" || (targetAll == true && i == 6))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/2_Tuesday.txt";
		if (target == "writeWedRevenue" || (targetAll == true && i == 7))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/3_Wednesday.txt";
		if (target == "writeThuRevenue" || (targetAll == true && i == 8))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/4_Thursday.txt";
		if (target == "writeFriRevenue" || (targetAll == true && i == 9))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/5_Friday.txt";
		if (target == "writeSatRevenue" || (targetAll == true && i == 10))
			filePath = 
			"../Sports Center Booking Management System/Booking Data/Revenue Data/6_Saturday.txt";
		
		ofstream target(filePath.c_str());
		target << value;
		target.close();
	}
}
void advSettings() {
  	cout << " +-----------------------------------------+" << endl;
	cout << " |            [!] WARNING! [!]             |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " |     ARE YOU REALLY SURE YOU WANT TO     |" << endl;
	cout << " |    DELETE ALL BOOKING/REVENUE DATA ?    |" << endl;
	cout << " |          (This can't be undone)         |" << endl;
	cout << " |                                         |" << endl;
	cout << " |             [Y] Yes  [N] No             |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << endl;
	do {
		pressedKey = getch();
		if (pressedKey == 27 || tolower(pressedKey) == 'n') 
			main();
	} while (pressedKey != 13 && tolower(pressedKey) != 'y');

	char pathDetailedData[256] = 
	"../Sports Center Booking Management System/Booking Data/Detailed Data";
	char pathTableData[256] = 
	"../Sports Center Booking Management System/Booking Data/Table Data";
	DIR *detailedDataFolder = opendir(pathDetailedData);
	DIR *tableDataFolder = opendir(pathTableData);
	struct dirent *next_file;
  	char filepath[256];

	while ((next_file = readdir(detailedDataFolder)) != NULL) {
   		sprintf(filepath, "%s/%s", pathDetailedData, next_file->d_name);
       	remove(filepath);
   	}
   	closedir(detailedDataFolder);
   	while ((next_file = readdir(tableDataFolder)) != NULL) {
   		sprintf(filepath, "%s/%s", pathTableData, next_file->d_name);
     	remove(filepath);
    }
    closedir(tableDataFolder);
	
	writeData(true, " ", 0); // Resets data to value 0

    cout << " [i] FILES SUCCESSFULLY DELETED!\n" << endl;
	cout << " [ESC] BACK TO MAIN MENU" << endl;
	waitEsc();
	main();
}

void endPrompt() {
	cout << " +-----------------------------------------+" << endl;
	cout << " |            [!] WARNING! [!]             |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << " |      ARE YOU SURE YOU WANT TO EXIT?     |" << endl;
	cout << " |                                         |" << endl;
	cout << " |            [Y] Yes   [N] No             |" << endl;
	cout << " +-----------------------------------------+" << endl;
	cout << endl;
	do {
		pressedKey = getch();
		if (pressedKey == 27 || tolower(pressedKey) == 'n') 
			main();
	} while (pressedKey != 13 && tolower(pressedKey) != 'y');
}
void waitEsc() {
	do {
		pressedKey = getch();
	} while ((pressedKey) != 27);
}
