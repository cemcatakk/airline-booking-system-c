#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define MAX 1000
#define WORD 60
char *adminPassword;
int flightCount,passengerCount,bookingCount;
struct Flight{
	char flightCode[WORD];
	char Airlines[WORD];
	char depAirport[WORD];
	char destAirport[WORD];
	char departureTime[WORD];
	char arrivalTime[WORD];
	int pasCapacity;
	int *seats;
}flights[MAX];
struct Passenger{
	int id;
	char Name[WORD];
	char Surname[WORD];
}passengers[MAX];
struct Booking{
	int bookingId;
	struct Flight flight;
	struct Passenger passenger;
	int seatNumber;
}bookings[MAX];


int searchPassenger(int id){
	int i;
	for(i=0;i<passengerCount;i++){
		if(passengers[i].id==id){
			return i;
		}
	}
	return -1;
}
int flightExists(char flightCode[WORD]){
	int i;
	for(i=0;i<flightCount;i++){
		if(flights[i].seats!=NULL){
			if(strcmp(flights[i].flightCode,flightCode)==0){
				return i;
			}
		}
	}
	return -1;
}
void saveDatabase(){
	FILE* f = fopen("flights.txt","w");
	int i,j;
	for(i=0;i<flightCount;i++){
		if(flights[i].seats!=NULL){
			fprintf(f,"%s %s %s %s %s %s %d\n",flights[i].flightCode,flights[i].Airlines,flights[i].depAirport,flights[i].destAirport
			,flights[i].departureTime,flights[i].arrivalTime,flights[i].pasCapacity);
			for(j=0;j<flights[i].pasCapacity;j++){
				fprintf(f,"%d ",flights[i].seats[j]);
			}
			fprintf(f,"\n");
		}
	}
	fclose(f);
	f = fopen("passengers.txt","w");
	for(i=0;i<passengerCount;i++){
		fprintf(f,"%d %s %s\n",passengers[i].id,passengers[i].Name,passengers[i].Surname);
	}
	fclose(f);
	f=fopen("bookings.txt","w");
	for(i=0;i<bookingCount;i++){
		if(strcmp(bookings[i].flight.flightCode,"")!=0){
			fprintf(f,"%d %s %d %d\n",bookings[i].bookingId,bookings[i].flight.flightCode,bookings[i].passenger.id,bookings[i].seatNumber);
		}
	}
	fclose(f);
	printf("Data has been written successfully\n");
}
void loadDatabase(){
	FILE* f = fopen("flights.txt","r");
	int i;
	flightCount=0;
	while(!feof(f)){
		fscanf(f,"%s %s %s %s %s %s %d\n",flights[flightCount].flightCode,flights[flightCount].Airlines,flights[flightCount].depAirport,flights[flightCount].destAirport
			,flights[flightCount].departureTime,flights[flightCount].arrivalTime,&flights[flightCount].pasCapacity);
			flights[flightCount].seats=(int*)malloc(sizeof(int)*flights[flightCount].pasCapacity);
		for(i=0;i<flights[flightCount].pasCapacity;i++){
			fscanf(f,"%d ",&flights[flightCount].seats[i]);
		}
		fscanf(f,"\n");
		flightCount++;
	}
	fclose(f);
	passengerCount=0;
	f=fopen("passengers.txt","r");
	while(!feof(f)){
		fscanf(f,"%d %s %s\n",&passengers[passengerCount].id,passengers[passengerCount].Name,passengers[passengerCount].Surname);
		passengerCount++;
	}
	fclose(f);
	f=fopen("bookings.txt","r");
	bookingCount=0;
	char fcode[WORD];
	int pid,bid,seatNo;
	while(!feof(f)){
		fscanf(f,"%d %s %d %d\n",&bid,fcode,&pid,&seatNo);
		if(flightExists(fcode)!=-1&&searchPassenger(pid)!=-1){
			bookings[bookingCount].flight=flights[flightExists(fcode)];
			bookings[bookingCount].passenger=passengers[searchPassenger(pid)];
			bookings[bookingCount].bookingId=bookingCount;
			bookings[bookingCount].seatNumber=seatNo;
			bookingCount++;
		}
		
	}
}

void printFlight(struct Flight flight){
	printf("%-15s %-15s %-15s %-15s %-15s %-15s %-15d\n",flight.flightCode,flight.Airlines,flight.depAirport,flight.destAirport,flight.departureTime,flight.arrivalTime,flight.pasCapacity);
}
void printPassenger(struct Passenger pass){
	printf("%-15d %-15s %-15s\n",pass.id,pass.Name,pass.Surname);
}
void printBooking(struct Booking booking){
	printf("Booking ID: %d\n",booking.bookingId);
	printf("Flight Info\n");
	printf("%-15s %-15s %-15s %-15s %-15s %-15s %-15s\n\n","Flight Code","Airlines","Dep. Airport","Dest. Airport","Dep. Time","Arr. Time","Pas. Capacity");
	printFlight(booking.flight);
	printf("Passenger Info\n");
	printf("%-15s %-15s %-15s\n","Passenger ID","Name","Surname");
	printPassenger(booking.passenger);
	printf("Seat Number: %d\n",booking.seatNumber);
}
void listBookings(){
	printf("All Booking Information\n");
	int i;
	for(i=0;i<bookingCount;i++){
		if(strcmp(bookings[i].flight.flightCode,"")!=0){
			printBooking(bookings[i]);
		}
	}
}
void listFlights(){
	int i;
	printf("%-15s %-15s %-15s %-15s %-15s %-15s %-15s\n\n","Flight Code","Airlines","Dep. Airport","Dest. Airport","Dep. Time","Arr. Time","Pas. Capacity");
	for(i=0;i<flightCount;i++){
		if(flights[i].seats!=NULL){
			printFlight(flights[i]);
		}
	}
}
void listPassengers(){
	int i;
	printf("%-15s %-15s %-15s\n","Passenger ID","Name","Surname");
	for(i=0;i<passengerCount;i++){
		if(strcmp(passengers[i].Name,"")!=0){
			printPassenger(passengers[i]);
		}
	}
}
void readAdminPass(){
	FILE* passFile = fopen("pass.txt","r");
	if(passFile){
		adminPassword=(char*)malloc(sizeof(char)*WORD);
		fscanf(passFile,"%s\n",adminPassword);
		printf(">>Admin password has been read.\n");
		fclose(passFile);
	}
	else{
		printf("One of database files(pass.txt) could not read.\n");
		exit(-1);
	}
}
void writeAdminPass(){
	FILE* passFile = fopen("pass.txt","w");
	if(passFile){
		fprintf(passFile,"%s",adminPassword);
		fclose(passFile);
		printf("Admin password has been updated\n");
	}
	else{
		printf("Error writing to file 'pass.bin'\n");
	}
}
void changePassword(){
	if(login()==1){
		char newPass[WORD],checkPass[WORD];
		printf("Enter new password:");
		scanf("%s",newPass);
		printf("Enter the password again:");
		scanf("%s",checkPass);
		if(strcmp(newPass,checkPass)==0){
			strcpy(adminPassword,newPass);
			writeAdminPass();
		}
		else{
			printf("Passwords does not match!\n");
		}
	}
	else{
		printf("Wrong password.\n");
	}
}
int login(){
	char input[WORD];
	printf("Enter password:");
	scanf("%s",input);
	if(strcmp(input,adminPassword)==0){
		return 1;
	}
	else{
		return 0;
	}
}
void addBooking(){
	int passengerID;
	printf("Enter passenger ID:"),
	scanf("%d",&passengerID);
}

void addFlight(){
	printf("Enter flight code:");
	char fcode[WORD];
	scanf("%s",fcode);
	if(flightExists(fcode)==-1){
		strcpy(flights[flightCount].flightCode,fcode);
		printf("Enter Airline Name:");
		scanf("%s",flights[flightCount].Airlines);
		printf("Enter Departure Airport:");
		scanf("%s",flights[flightCount].depAirport);
		printf("Enter Departure Time(hh:mm):");
		scanf("%s",flights[flightCount].departureTime);
		printf("Enter Destination Airport:");
		scanf("%s",flights[flightCount].destAirport);
		printf("Enter Arrival Time(hh:mm):");
		scanf("%s",flights[flightCount].arrivalTime);
		printf("Enter seat count:");
		scanf("%d",&flights[flightCount].pasCapacity);
		flights[flightCount].seats=(int*)malloc(sizeof(int)*flights[flightCount].pasCapacity);
		int i=0;
		for(i=0;i<flights[flightCount].pasCapacity;i++){
			flights[flightCount].seats[i]=0;
		}
		printf("Flight has been added.\n");
		flightCount++;
	}
	else{
		printf("A flight with %s code already exists.\n",fcode);
	}
}
void editFlight(){
	printf("Enter flight code:");
	char fcode[WORD];
	scanf("%s",fcode);
	int findex=flightExists(fcode);
	if(findex!=-1){
		printf("Enter New Airline Name:");
		scanf("%s",flights[findex].Airlines);
		printf("Enter New Departure Airport:");
		scanf("%s",flights[findex].depAirport);
		printf("Enter New Departure Time(hh:mm):");
		scanf("%s",flights[findex].departureTime);
		printf("Enter New Destination Airport:");
		scanf("%s",flights[findex].destAirport);
		printf("Enter New Arrival Time(hh:mm):");
		scanf("%s",flights[findex].arrivalTime);
		printf("Enter New seat count:");
		scanf("%d",&flights[findex].pasCapacity);
		flights[findex].seats=(int*)malloc(sizeof(int)*flights[findex].pasCapacity);
		int i=0;
		for(i=0;i<flights[findex].pasCapacity;i++){
			flights[findex].seats[i]=0;
		}
		printf("Flight has been updated.\n");
	}
	else{
		printf("Flight with code %s could not found.\n",fcode);
	}
}
void deleteFlight(){
	char fcode[WORD];
	printf("Enter flight code:");
	scanf("%s",fcode);
	int findex=flightExists(fcode);
	if(findex!=-1){
		flights[findex].seats=NULL;
	}
	else{
		printf("Flight with code %s could not found.\n",fcode);
	}
}
void adminMenu(){
	if(login()==1){
		int choice;
		while(1){
			printf("1-Add Flight\n2-Edit Flight\n3-Delete Flight\n4-List Flights\n5-List Bookings\n6-Change password and return to main menu\n>>");
			scanf("%d",&choice);
			if(choice==1){
				addFlight();
				saveDatabase();
				loadDatabase();
			}
			else if(choice==2){
				editFlight();
				saveDatabase();
				loadDatabase();
			}
			else if(choice==3){
				deleteFlight();
				saveDatabase();
				loadDatabase();
			}
			else if(choice==4){
				listFlights();
			}
			else if(choice==5){
				listBookings();
			}
			else if(choice==6){
				changePassword();
				break;
			}
			else{
				printf("Invalid choice\n");
			}
		}
	}
}
int searchFlights(char depAirport[WORD],char destAirport[WORD]){
	int i,found=-1;
	printf("%-15s %-15s %-15s %-15s %-15s %-15s %-15s\n\n","Flight Code","Airlines","Dep. Airport","Dest. Airport","Dep. Time","Arr. Time","Pas. Capacity");
	for(i=0;i<flightCount;i++){
		if(flights[i].seats!=NULL){
			if(strcmp(flights[i].depAirport,depAirport)==0&&strcmp(flights[i].destAirport,destAirport)==0){
				printFlight(flights[i]);
				found=1;
			}
		}
	}
	return found;
}
int isFull(struct Flight flight){
	int count=0,i;
	for(i=0;i<flight.pasCapacity;i++){
		if(flight.seats[i]==1){
			count++;
		}
	}
	if(flight.pasCapacity==count)return 1;
	else return -1;
}
void bookFlight(){
	char destAirport[WORD];
	char depAirport[WORD];
	printf("Enter departure airport:");
	scanf("%s",depAirport);
	printf("Enter destination airport:");
	scanf("%s",destAirport);
	printf("Searching for available flights..\n");
	if(searchFlights(depAirport,destAirport)!=-1){
		char fcode[WORD];
		printf("Enter flight code to book:");
		scanf("%s",fcode);
		int findex=flightExists(fcode);
		if(findex!=-1){
			if(isFull(flights[findex])!=1){
				int seatNo;
				while(1){
					printf("Select a seat number between 0 and %d\n>>",flights[findex].pasCapacity);
					scanf("%d",&seatNo);
					if(flights[findex].seats[seatNo]==1){
						printf("This seat is already booked.\n");
					}
					else break;
				}
				printf("Have you ever booked before? 1-Yes 2-No\n");
				int bookedBefore;
				scanf("%d",&bookedBefore);
				if(bookedBefore==1){
					printf("Enter your ID:");
					int pid;
					scanf("%d",&pid);
					if(strcmp(passengers[pid].Name,"")!=0){
						flights[findex].seats[seatNo]=1;
						bookings[bookingCount].passenger=passengers[pid];
						bookings[bookingCount].flight=flights[findex];
						bookings[bookingCount].seatNumber=seatNo;
						bookings[bookingCount].bookingId=bookingCount;
						bookingCount++;
						printf("You have successfully booked to flight %s!\n",fcode);
						return;
					}
				}
				else{
				printf("Enter your name: ");
				scanf("%s",passengers[passengerCount].Name);
				printf("Enter your surname: ");
				scanf("%s",passengers[passengerCount].Surname);
				passengers[passengerCount].id=passengerCount;
				printf("Your ID is: %d\n",passengerCount);
				flights[findex].seats[seatNo]=1;
				bookings[bookingCount].passenger=passengers[passengerCount];
				bookings[bookingCount].flight=flights[findex];
				bookings[bookingCount].seatNumber=seatNo;
				bookings[bookingCount].bookingId=bookingCount;
				bookingCount++;
				passengerCount++;
				printf("You have successfully booked to flight %s!\n",fcode);
				}
				
			}
			else printf("No seats left in this flight\n");
		}
		else{
			printf("No flight found with code %s\n",fcode);
		}
	}
	else printf("No flight found\n");
	
}
void cancelBooking(){
	printf("Enter booking ID to cancel\n>>");
	int bookingID;
	scanf("%d",&bookingID);
	if(bookings[bookingID].bookingId!=bookingID){
		printf("Booking could not found.\n");
	}
	else{
		strcpy(bookings[bookingID].flight.flightCode,"");
		bookings[bookingID].bookingId=-1;
		printf("Booking canceled.\n");
	}
}
void listBookingsFiltered(){
	int pid;
	printf("Enter passenger ID:");
	scanf("%d",&pid);
	if(searchPassenger(pid)!=-1)
	{
	printf("All Booking Information of Passenger ID: %d\n",pid);
	int i;
	for(i=0;i<bookingCount;i++){
		if(bookings[i].flight.seats!=NULL){
			if(bookings[i].passenger.id==pid){
				printBooking(bookings[i]);
			}
		}
	}
	}
	else printf("Passenger could not found.\n");
	
}
void passengerMenu(){
	int choice;
	while(1){
		printf("1-Book Flight\n2-Cancel Booking\n3-List Bookings\n4-Return to Main Menu\n>>");
		scanf("%d",&choice);
		if(choice==1){
			bookFlight();
			saveDatabase();
			loadDatabase();
		}
		else if(choice==2){
			cancelBooking();
			saveDatabase();
			loadDatabase();
		}
		else if(choice==3){
			listBookingsFiltered();
		}
		else if(choice==4){
			printf("...\n");
			break;
		}
		else{
			printf("Invalid choice\n");
		}
	}
}
int main(){
	flightCount=0;passengerCount=0;bookingCount=0;
	loadDatabase();
	readAdminPass();
	printf("---Welcome To Airline Reservation System---\n\n");
	int choice;
	while(1)
	{
		printf("1-Admin Menu\n2-Passenger Menu\n3-Exit\n>>");
		scanf("%d",&choice);
		if(choice==1){
			adminMenu();
		}
		else if(choice==2){
			passengerMenu();
		}
		else if(choice==3){
			printf("Goodbye!\n");
			break;
		}
		else printf("Invalid choice\n");
	}
	saveDatabase();
}
