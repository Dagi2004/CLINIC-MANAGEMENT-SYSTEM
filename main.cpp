#include <iostream>
#include<iomanip>
#include<cstring>
#include "sqlite3.h"
using namespace std;

sqlite3 *db; // sqlite3 object pointer
sqlite3_stmt * stmt; // to hold the prepared SQL statement object
int result;
// Functions

void connection();
void authenticate();
void patientinfo ();
void  staffinfo();
void patientsearch();
void staffsearch();
void medicalRecord();
void PrescriptionRecord();
void PatientUpdate();
void StaffUpdate();
void deletePatientRecord();
void deleteStaffRecord();
int main()
{
   sqlite3_open("modified database.db", &db); // Create or open new database
    connection(); // call connection function



     cout << "\t\t\t  -----------------------------" << endl;
    cout << "\t\t\t  |        Staff Login         |" << endl;
    cout << "\t\t\t  -----------------------------" << endl;

     authenticate();
      cout << "\t\t\t ---------------------------------" << endl;
    cout << "\t\t\t |   WELCOME TO OUR UNIVERSITY CLINIC | " << endl;
    cout << "\t\t\t  ----------------------------------" << endl;

    cout << "\t\t\t 1) Register Patient" << endl;
    cout << "\t\t\t 2) Register Staff" << endl;
    cout << "\t\t\t 3) Insert medical record" << endl;
    cout << "\t\t\t 4) Search for a Patient || Staff " << endl;
    cout << "\t\t\t 5) Update Record " << endl;
    cout << "\t\t\t 6) Delete record " << endl;
    cout << "\t\t\t-----------------------------" << endl;


    // Ask the user what they want to do
    cout << "\n\t\t\tChoose an option: [1/2/3/4/5/6/7]" << endl;
    again:
int choice;

cout << "\t\t\tEnter your choice: ";
cin >> choice;

switch (choice) {

    case 1:
        patientinfo(); // Insert patient data to the table
        break;

    case 2:
        staffinfo();
        break;

    case 3:
        medicalRecord();
        break;

    case 4:
        int searchChoice;
        cout << "\t\t\t 1.Search by Patient :" << endl;
        cout << "\t\t\t 2.Search by Staff :" << endl;
        cout << "\t\t\t Choice: ";
        cin >> searchChoice;
        switch (searchChoice) {
            case 1:
                patientsearch();
                break;
            case 2:
                staffsearch();
                break;

        }


    default:
        cout << "Invalid choice. Please try again." << endl;
        goto again;

        case 5:
            int updateinfo;
             cout << "\t\t\t-----------------------------" << endl;
          cout << "\t\t\t | UPDATE RECORD  | " << endl;
           cout << "\t\t\t-----------------------------" << endl;
            cout<<"\t\t\t Press 1 to update Patient Record " <<endl;
            cout<<"\t\t\t Press 2 to update staff Record " <<endl;
            cout << "\t\t\t Choice: " ;
            cin>>updateinfo;
            switch(updateinfo){
            case 1:
                PatientUpdate();
                break;
            case 2:
                StaffUpdate();
                break;
            }
            break;
            /* case 2:
               StaffUpdate();
                break;
            }*/
            case 6:
                int deleterecord;
                cout<<"\t\t\t 1.Delete Patient Record:"<<endl;
                cout<<"\t\t\t 2.Delete Staff Record:"<<endl;
                 cout << "\t\t\t Choice: " ;
                cin>>deleterecord;
                switch(deleterecord){
                    case 1:
                deletePatientRecord();
                break;
                case 2:
                deleteStaffRecord();
                break;
                }
                break;
}

sqlite3_close(db);
return 0;
}
void connection()
{
    if (sqlite3_open("modified database.db", &db) == SQLITE_OK)
    {
        cout << "the database is open" << endl;
    }
}

void authenticate()
{
    input:
    // Get input from the user
    int id;
    string password;

    cout << "\t\t\t   Enter your ID: ";
    cin >> id;
    cout << "\t\t\t   Enter your Password: ";
    cin >> password;


    cout << "\t\t\t  -----------------------------" << endl;

    // compare the input from the user
    string query = "SELECT StaffPassword FROM STAFF WHERE StaffID = ?";

    // if correct login
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    sqlite3_bind_int(stmt,1,id);

    // In case of error
    if(result!=SQLITE_OK)
        cout<<"Error " << sqlite3_errmsg(db) << endl;

    bool found = false;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        found = true;

        // Retrieve column values
        const char * pass = password.c_str();
        const char * pass1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));

        if (strcmp(pass1,pass) == 0)
        {

           cout <<  "\t\t\t\t Login successful\n" << endl;
           return;
        }
        else
        {
            // if not correct
            cout << "\t\t\t The entry is incorrect. Try again!\n" << endl;
            goto input;
        }
    }

    // if not correct or found
    if (!found)
    {
        cout << "\t\t\t The entry is not found!\n" << endl;
        goto input;
    }


    sqlite3_finalize(stmt);
}



// staff data
void staffinfo(){
    input:
        string StaffName,StaffPosition,StaffPassword,  StaffAddress ;
        int Contact;
    cout << "\t\t\t-----------------------------" << endl;
 cout << "\t\t\t | Staff RECORD  | " << endl;

    cout << "\t\t\t-----------------------------" << endl;
    cout<<"\t\t\tEnter Staff Name:";
    cin>>StaffName;

        cout<<"\t\t\tEnter Staff Position:";
    cin>>StaffPosition;


        cout<<"\t\t\tEnter Staff Contact:";
    cin>> Contact;
    cin.ignore();


        cout<<"\t\t\tEnter StaffAddress:";
    cin>>StaffAddress;

      cout<<"\t\t\tEnter Staff Password: ";
    cin>>StaffPassword;

 string query = "INSERT INTO Staff(StaffName, StaffPosition, Contact, StaffAddress, StaffPassword) VALUES (?,?,?,?,?);";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
    sqlite3_bind_text(stmt, 1, StaffName.c_str(), StaffName.length(), SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 2, StaffPosition.c_str(), StaffPosition.length(), SQLITE_TRANSIENT);
sqlite3_bind_int(stmt, 3, Contact);
sqlite3_bind_text(stmt, 4, StaffAddress.c_str(), StaffAddress.length(), SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 5, StaffPassword.c_str(), StaffPassword.length(), SQLITE_TRANSIENT);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    char y;
    cout << endl << "Do you want to add another patient? (Y/N): ";
    cin >> y;

    if(toupper(y)=='Y')
        goto input;

    else{
     if (result == SQLITE_OK)
            cout << "The Data inserted successfully." << endl;
            else
                cout<<" Error: " << sqlite3_errmsg(db) << endl;
    }
}


// Insert patient info
void patientinfo()
{
    input:
    string patientName, department, dateOfBirth, gender;
    int contact;
    int numberofPatient;
    cout << "\t\t\t-----------------------------" << endl;
 cout << "\t\t\t | Patient Registration   | " << endl;
    cout << "\t\t\t-----------------------------" << endl;
   cout << "\t\t\tEnter Patient Name: ";
    cin >> patientName;

    cout << "\t\t\tEnter Department: ";
      cin.ignore();
        getline(cin, department);

    cout <<  "\t\t\tEnter Date of Birth: ";
    cin >> dateOfBirth;

    cout <<  "\t\t\tEnter Gender: ";
    cin >> gender;

    cout <<  "\t\t\tEnter Contact: ";
    cin >> contact;




    string query = "INSERT INTO Patient(PatientName, Department, DateofBirth, Gender, Contact) VALUES (?,?,?,?,?);";

    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL); // it compiles the SQL query and stores it in the stmt statement object.
    sqlite3_bind_text(stmt, 1, patientName.c_str(), patientName.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, department.c_str(), department.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, dateOfBirth.c_str(), dateOfBirth.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, gender.c_str(), gender.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 5, contact);

    sqlite3_step(stmt); // executes the prepared statement
    sqlite3_finalize(stmt); //  finalizes the prepared statement, freeing any resources associated with it

    // Ask the user if they want to do it again
    char y;
    cout << endl << "Do you want to add another patient? (Y/N): ";
    cin >> y;

    if (toupper(y)=='Y')
        goto input;
    else
    {
        if (result == SQLITE_OK)
            cout << "The Data inserted successfully.";
        else
            cout << "ERROR: " << sqlite3_errmsg(db) << "\n";
    }

    }



void medicalRecord(){
    input:
    string Diagnosis,Treatment,dateOfVisit;
    int RecordID,PatientID;
  cout << "\t\t\t-----------------------------" << endl;
 cout << "\t\t\t | Insert Medical Record   | " << endl;
    cout << "\t\t\t-----------------------------" << endl;
        cout<<"\t\t\tEnter DateofVisit :" ;
        cin >> dateOfVisit;
        cin.ignore();


          cout << "\t\t\tEnter Diagnosis :" ;

        getline(cin, Diagnosis);


        cout << "\t\t\tEnter Treatment :" ;
        getline(cin, Treatment);

         cout<<"\t\t\tEnter PatientID :" ;
        cin>>PatientID;

    string query = "INSERT INTO MedicalRecord( DateOfVisit, Diagnosis, Treatment, PatientID) VALUES (?,?,?,?);";

    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, dateOfVisit.c_str(),dateOfVisit.length(),SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, Diagnosis.c_str(), Diagnosis.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, Treatment.c_str(), Treatment.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, PatientID);

                 sqlite3_step(stmt);
                sqlite3_finalize(stmt);

              char y;
    cout << endl << "Do you want to add another patient? (Y/N): ";
    cin >> y;
    if (toupper(y) == 'Y') {
        medicalRecord(); // Call the function recursively instead of using goto
    }  else
    {
        if (result == SQLITE_OK)
            cout << "The Data inserted successfully.";
        else
            cout << "ERROR: " << sqlite3_errmsg(db) << "\n";
    }
}
void PrescriptionRecord() {
    int PrescriptionID, MRecordID, PatientID, StaffID;
    string Medication_Name, Dosage_Instructions, Duration, Prescription_Date;

    string query = "INSERT INTO Prescription(PrescriptionID, Medication_Name, Dosage_Instructions, Duration, Prescription_Date, MRecordID, StaffID, PatientID) VALUES (?,?,?,?,?,?,?,?);";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, PrescriptionID);
    sqlite3_bind_text(stmt, 2, Medication_Name.c_str(), Medication_Name.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, Dosage_Instructions.c_str(), Dosage_Instructions.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, Duration.c_str(), Duration.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, Prescription_Date.c_str(), Prescription_Date.length(), SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 6, MRecordID);
    sqlite3_bind_int(stmt, 7, StaffID);
    sqlite3_bind_int(stmt, 8, PatientID);

}

void patientsearch()

{

    // Input user ID OR Name
    int choice;
    cout << "\t\t\t 1) Search by ID"  <<endl;
    cout << "\t\t\t 2) Search by Name" <<endl;
    cout <<  "\t\t\t Choice: ";
    cin >> choice;

    // search based on the choice
    switch (choice){
    case 1: {
        int id;
         cout<<"\t\t\tEnter Patient ID: ";
        cin>>id;
        cin.ignore();
               string query="SELECT *FROM Patient WHERE PatientID=?;";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
            sqlite3_bind_int(stmt,1,id);
            break;
    }
         case 2: {
        string name;
         cout<<"\t\t\tEnter Patient Name  " << endl;
          cin.ignore();
        getline(cin,name);

       // Preapring Query
        string query = "SELECT * FROM Patient WHERE PatientName = ?;";
        // Bind the parameter value

    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
            sqlite3_bind_text(stmt,1,name.c_str(),name.length(),SQLITE_TRANSIENT);
            break;
    }
          default:
            cout<< "\t\t\t Invalid choice Please try again " << endl;
         patientsearch();

    }
     if(result!=SQLITE_OK)
cout<<"Error " << sqlite3_errmsg(db) << endl;

 else {
  bool found = false;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW){
        found = true;
        if (found) {

            cout << "------------------------------------------- Employee Search Data ----------------------------------------------" << endl;
            cout << "\n|   Patient-ID   |   Patient-Name   |   DEPARTMENT   |   DATEOFBIRTH   |   GENDER   |   CONTACT   |" << endl;
            cout << "|------------------|----------------|----------------|-----------------|------------|--------------|" << endl;

        }

        // Retrieve column values
        cout << "|" << setw(18) << left << sqlite3_column_text(stmt, 0) << "|";
        cout << setw(15) << left << sqlite3_column_text(stmt, 1) << "|";
        cout << setw(15) << left << sqlite3_column_text(stmt, 2) << "|";
        cout << setw(16) << left << sqlite3_column_text(stmt, 3) << "|";
        cout << setw(11) << left << sqlite3_column_text(stmt, 4) << "|";
        cout << setw(13) << left << sqlite3_column_text(stmt, 5) << "|" << endl;


  if (!found)
            cout << "The entry is not found!";

        sqlite3_finalize(stmt);

    }
}
}

// Search Staff
void staffsearch(){
     int choice;
    cout << "\t\t\t 1) Search by ID:" <<endl;
    cout << "\t\t\t 2) Search by Name:" <<endl;
        cout << "Choice: ";
    cin >> choice;
     switch (choice){
    case 1: {
        int id;
         cout<<"\t\t\tEnter Staff ID " ;
        cin>>id;
        cin.ignore();
        string query="SELECT *FROM Staff WHERE StaffID=?;";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
            sqlite3_bind_int(stmt,1,id);
            break;
        }
          case 2: {
        string name;
         cout<<"\t\t\tEnter Staff Name  " << endl;
          cin.ignore();
        getline(cin,name);

       // Preapring Query
        string query = "SELECT * FROM Staff WHERE StaffName = ?;";
        // Bind the parameter value

    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
            sqlite3_bind_text(stmt,1,name.c_str(),name.length(),SQLITE_TRANSIENT);
            break;
    }
          default:
            cout<< "Invalid choice Please try again " << endl;
         staffsearch();

    }
   if(result!=SQLITE_OK)
cout<<"Error " << sqlite3_errmsg(db) << endl;

 else {
  bool found = false;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        found = true;
        if (found) {

            cout << "------------------------------------------- Employee Search Data ----------------------------------------------" << endl;
            cout << "\n|   Staff-ID   |   Staff-Name  |   Staff Position |    CONTACT     |   Address    |" << endl;
            cout << "|------------------|----------------|----------------|-----------------|------------|" << endl;
           // found = false; // Reset the flag after printing the table header
        }

        // Retrieve column values
        cout << "|" << setw(18) << left << sqlite3_column_text(stmt, 0) << "|";
        cout << setw(15) << left << sqlite3_column_text(stmt, 1) << "|";
        cout << setw(15) << left << sqlite3_column_text(stmt, 2) << "|";
        cout << setw(16) << left << sqlite3_column_text(stmt, 3) << "|";
        cout << setw(11) << left << sqlite3_column_text(stmt, 4) << "|";
        cout << setw(11) << left << sqlite3_column_text(stmt, 5) << "|";


   if (!found)
            cout << "The entry is not found!";

        sqlite3_finalize(stmt);

    }
}
}
void PatientUpdate(){
    again:
    int id;
     string choice;
     int Contact;
     cout<< "\t\t\tEnter Patient id: " ;
     cin>>id;
    cout << "\t\t\t-----------------------------" << endl;
    cout<< "\t\t\tWhich of the following data you want to update " <<endl;
    cout<< "\t\t\t 1.Patient Name" << "\n" ;
    cout<< "\t\t\t 2.Patient Department" << "\n";
    cout<< "\t\t\t 3.Patient DateofBirth" << "\n" ;
    cout<< "\t\t\t 4.Patient Contact" << "\n" ;
    cout << "\t\t\t-----------------------------" << endl;
    string PatientName, Department, DateofBirth;

 cout << "\n\t\t\t Choose an option:[PatientName/Department/DateofBirth/Contact]" << endl;
 cout << "\t\t\t Enter your choice: ";
  cin>>choice;

    if(choice=="PatientName"){
        cout << "\t\t\tEnter Updated Patient Name: ";
    cin >> PatientName;
    string query = "UPDATE Patient SET PatientName = ? WHERE PatientID = ?";
        result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, PatientName.c_str(), PatientName.length(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, id);
    }

    else if(choice=="Department"){
        cout << "\t\t\tEnter Updated Patient Department: ";
 cin >> Department;
 string query = "UPDATE Patient SET Department = ? WHERE PatientID = ?";
        result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, Department.c_str(), Department.length(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, id);
    }

    else if(choice=="DateofBirth"){
        cout << "\t\t\tEnter Updated Date of Birth: ";
    cin >> DateofBirth;
     string query = "UPDATE Patient SET DateofBirth = ? WHERE PatientID = ?";
        result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, DateofBirth.c_str(), DateofBirth.length(), SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, id);
    }


    else if(choice=="Contact"){
    cout << "\t\t\tEnter Updated Contact: ";
    cin >> Contact;
    string query = "UPDATE Patient SET Contact = ? WHERE PatientID = ?";
        result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_int(stmt, 1, Contact);
        sqlite3_bind_int(stmt, 2, id);
    }
    else {
        cout << "\t\t\tInvalid choice." << endl;
       goto again;
    }


    // checks if user entered values(patientName,department,dateOfBirth and contact are not empty and if the condition is true, it appends the following string to the query.
 /*if (!PatientName.empty()) {
    query += "`PatientName` = '" + PatientName + "', ";
}
else if(!Department.empty()){
    query+="Department = '" + Department  + "', ";

}
else if(!DateofBirth.empty()){
    query+="DateofBirth = '" + DateofBirth  + "', ";

}
else if(Contact!=0){
  query += "Contact = '" + to_string(Contact) + "', ";
} */
// Removes Trailing comas and space since its not accepted in SQLITE update syntax


//it will update based on  the PATIENTid entered from user


 // it compiles the SQL query and stores it in the stmt statement object.

 if(result!=SQLITE_OK){
    cout<<"Error " << sqlite3_errmsg(db) << endl;
 }


result = sqlite3_step(stmt);
    if (result == SQLITE_DONE) {
        cout << "\t\t\t Patient record updated successfully." << endl;
    } else {
        cout << "\t\t\t Error updating patient record: " << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}

void StaffUpdate(){
int id;
     string choice ;
     int Contact;
     cout<< "\t\t\tEnter Staff id: " ;
     cin>>id;
   cout << "\t\t\t-----------------------------" << endl;
    cout<< "\t\t\tWhich of the following data you want to update " <<endl;
    cout<< "\t\t\t 1.Staff Name" <<endl;
    cout<< "\t\t\t 2.Staff Position" <<endl ;
    cout<< "\t\t\t 3.Staff Address" <<endl;
    cout<< "\t\t\t 4.Staff Contact" <<endl ;
    cout << "\t\t\t-----------------------------" << endl;
    string StaffName, StaffPosition, StaffAddress;

cout << "\n\t\t\tChoose an option:[Name/Position/Address/Contact]" << endl;
 cout << "Enter your choice: ";
  cin>>choice;

    if(choice=="Name"){
        cout << "\t\t\tEnter Updated Staff Name: ";
    cin >> StaffName;
    }

    else if(choice=="Position"){
        cout << "\t\t\tEnter Updated Staff Position: ";
 cin >> StaffPosition;
    }

    else if(choice=="Address"){
        cout << "\t\t\tEnter Updated StaffAddress : ";
    cin >>StaffAddress;
    }


    else if(choice=="Contact"){
    cout << "\t\t\tEnter Updated Contact: ";
    cin >> Contact;
    }
    // Update table syntax
    string query = "UPDATE Staff SET ";

    // checks if user entered values(patientName,department,dateOfBirth and contact are not empty and if the condition is true, it appends the following string to the query.
 if (!StaffName.empty()) {
    query += "`StaffName` = '" + StaffName + "', ";
}
else if(!StaffPosition.empty()){
    query+="StaffPosition = '" + StaffPosition  + "', ";

}
else if(!StaffAddress.empty()){
    query+="StaffAddress = '" + StaffAddress  + "', ";

}
else if(Contact!=0){
  query += "Contact = '" + to_string(Contact) + "', ";
}
// Removes Trailing comas and space since its not accepted in SQLITE update syntax
query = query.substr(0, query.size() - 2);

//it will update based on  the PATIENTid entered from user
query+="WHERE StaffID = " + to_string(id) + ";";
result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL); // it compiles the SQL query and stores it in the stmt statement object.

 if(result!=SQLITE_OK){
    cout<<"Error " << sqlite3_errmsg(db) << endl;
 }

result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        cout << "Failed to execute the statement: " << sqlite3_errmsg(db) << endl;
    }
    else {
    cout << "Data Successfully Updated." << endl;
    }

    sqlite3_finalize(stmt);

}
void deletePatientRecord(){
int id;

  cout<< "\t\t\tEnter Patientid: " ;
     cin>>id;


       string query ="DELETE FROM Patient WHERE PatientID=?;";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_int(stmt,1,id);

        if(result!=SQLITE_OK){

cout<<"Error " << sqlite3_errmsg(db) << endl;
        }

        else{
 if (sqlite3_step(stmt)!=SQLITE_DONE) {
         cout << "Error deleting record." << endl;
 }
       else{
           cout << "The Data deleted successfully.";
       }
        }

     sqlite3_finalize(stmt);
}

void deleteStaffRecord(){
int id;
  cout<< "\t\t\tEnter Staffid: " ;
     cin>>id;

  string query ="DELETE FROM Staff WHERE StaffID=?;";
    result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, NULL);
        sqlite3_bind_int(stmt,1,id);

        if(result!=SQLITE_OK){

cout<<"Error " << sqlite3_errmsg(db) << endl;
        }

        else{
 if (sqlite3_step(stmt)!=SQLITE_DONE) {
         cout << "Error deleting record." << endl;
 }
       else{
           cout << "The Data deleted successfully.";
       }
        }

     sqlite3_finalize(stmt);
}


