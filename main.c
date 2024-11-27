/* Project Title: Automatic Time-Table Generator for educational institutions.
 * Description: This program generates unique time-table for each Class divisions, when sufficient inputs are provided.
 * Developed By: Anil Dewani
 */
#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdarg.h>
#define BGI_DIR "C:\\TC\\BGI" // Defines path to the graphics directory used by graphics.h library.

/*
 * Structure definition to store all variable related to a single project/standard.
 * nperoids_day - Total number of periods in a day.
 * nrecess - Total number of recess.
 * recess[] - Array which stores index of periods, which are recess periods.
 * nsubjects - Total number of subjects
 * nclasses - Total number of class divisions.
 * timetable[class][period][day] - It's an 3 dimensional array which stores the actual time-table. It stores index value of subject.
 * periods - Multi-dimension char array which stores string representation of each period. For example, 10:30 to 11:30
 * subjects - Multi-dimension char array which stores string representation of each subjects. For example, Mathematics.
 * classes - Multi-dimension char array which stores string representation of each class divisions. For example, A Division.
 * yesno - Stores whether subjects should be repeated within a single day or not. Possible values, 'y' and 'n'. Default value - 'y'.
 * projectname - Used to store a string representation for name of the project.
 
 This structure is typedef'd.
 'standard' is used to represent this structure within the whole code.
 */
 
typedef struct
{
	int nperiods_day,nrecess,recess[3],nsubjects,nclasses,timetable[10][10][6];
	char periods[10][20],subjects[10][20],classes[10][20],yesno,projectname[256];
}standard;

// An variable of this structure is globally declared to be used while recursion, to avoid stack overflows.
standard global_data;

//Function declarations
void outerbox(); 
void statusbar(char *); 
int show_menu(); 
standard new_timetable();
int show_projects();
standard generate_timetable(standard);
void check_unique(); 
int draw_timetable(int,int,standard);
standard load_project(int);
void delete_project(int);
int higheststrlen(char[10][20],int);
void save_project(standard);
void export_html(int, ...);

void main()
{
	int gd=DETECT,gm,flag=0,ch,project_choice,timetable_option;
	char *password;
	standard in_main;

	int class_division = 0;

	//initalize the graphics.
	initgraph(&gd,&gm,BGI_DIR);

	outerbox();

	//Get the password from the user and validate.
	setcolor(WHITE);
	statusbar("Type your password and hit enter.");
	do
	{
		gotoxy(25,16);
		//draws the rectangle box around the password prompt
		rectangle(getmaxx()/4,260,(getmaxx()*3)/4,getmaxy()-245);
		password = getpass("Enter Password: ");
		if(!strcmp(password,"secret"))
		{
			flag = 1;
			outerbox();
			statusbar("Login Succesfull. Please choose from options 1,2 or 3.");
		}
		else
		{
			statusbar("Invalid Password. Type your password again and hit enter");
		}
	}while(flag!=1);


	//Below code, handles the whole menu system.
	ch = show_menu();
	
	start:
	class_division = 0;
	switch(ch)
	{
		// New Time-table project case.
		case 1:
			in_main = new_timetable();
			in_main = generate_timetable(in_main);
			outerbox();
			statusbar("Unique Time-Tables Created Sucessfully.");
			timetable_option = draw_timetable(class_division,0,in_main);
			
			draw_new:
			class_division++;
			
			no_inc_new:
			
			//switch for options for the time table.. Like delete project, back to menu, export html, next timetable
			switch(timetable_option)
			{
				case 1:
					save_project(in_main);
					outerbox();
					statusbar("Please choose from options 1,2 or 3.");
					ch = show_menu();
					goto start;
				case 2:
					outerbox();
					statusbar("Please enter an option from 1,2 or 3.");
					ch = show_menu();
					goto start;
				case 3:
					export_html(0,in_main);
					outerbox();
					statusbar("Please enter an option from 1,2 or 3.");
					ch = show_menu();
					goto start;
				case 4:
					if(class_division>=in_main.nclasses-1)
					{
						cleardevice();
						outerbox();
						statusbar("Invalid Option Selected. Please enter an valid option");
		
						timetable_option = draw_timetable(in_main.nclasses-1,0,in_main);
						goto draw_new;
					}
					else
					{

						outerbox();
						statusbar("Unique Time-Tables Created Sucessfully.");
						timetable_option = draw_timetable(class_division,0,in_main);
						goto draw_new;
					}

				default:
						outerbox();
						statusbar("Invalid Option Selected. Please enter an valid option");
						timetable_option = draw_timetable(class_division-1,0,in_main);
						goto no_inc_new;
			}
		
		//case for showing existing projects
		case 2:
			project_choice = show_projects();
			if(project_choice == 0)
			{
				outerbox();
				statusbar("Please enter an option from 1,2 or 3.");
				ch = show_menu();
				goto start;
			}
			in_main = load_project(project_choice);
			timetable_option = draw_timetable(class_division,1,in_main);
			
			
			draw:
			class_division++;
			
			no_inc:

			//switch for options for the time table.. Like delete project, back to menu, export html, next timetable
			switch(timetable_option)
			{
				case 1:
					delete_project(project_choice);
					outerbox();
					statusbar("Please enter an option from 1,2 or 3.");
					ch = show_menu();
					goto start;
				case 2:
					outerbox();
					statusbar("Please enter an option from 1,2 or 3.");
					ch = show_menu();
					goto start;
				case 3:
					export_html(project_choice);
					outerbox();
					statusbar("Please enter an option from 1,2 or 3.");
					ch = show_menu();
					goto start;
				case 4:
					if(class_division>=in_main.nclasses-1)
					{
						cleardevice();
						outerbox();
						statusbar("Invalid Option Selected. Please enter an valid option");
		
						timetable_option = draw_timetable(in_main.nclasses-1,1,in_main);
						goto draw;
					}
					else
					{

						outerbox();
						statusbar("Unique Time-Tables Created Sucessfully.");
						timetable_option = draw_timetable(class_division,1,in_main);
						goto draw;
					}

				default:
						outerbox();
						statusbar("Invalid Option Selected. Please enter an valid option");
						timetable_option = draw_timetable(class_division-1,1,in_main);
						goto no_inc;
			}
		case 3:
			closegraph();
			exit();
		default:
			outerbox();
			statusbar("Invalid Option Selected. Please enter an option from 1,2 or 3.");
			ch = show_menu();
			goto start;
	}

	
}

// outerbox() is used to clear the screen and drow a border around the screen.
void outerbox()
{
	int maxx,maxy,left,top,right,bottom;
	
	cleardevice();
	
	maxx = getmaxx();
	maxy = getmaxy();

	left = 10;
	top = 10;
	right = maxx - 10;
	bottom = maxy - 10;


	setcolor(WHITE);
	rectangle(left,top,right,bottom);

	setfillstyle(SOLID_FILL,WHITE);
	bar(10,10,maxx-10,25);


	setcolor(BLACK);
	outtextxy(20,15,"Automatic Time-Table Creator v1.0");

	setcolor(WHITE);
	line(10,bottom-15,right,bottom-15);

}

/* statusbar() is used to write at lower part of the screen. Acts as status bar for giving instructions to the user.
 * It takes an character array as its input parameter.
 */
void statusbar(char *message)
{
	setfillstyle(SOLID_FILL,BLACK);
	bar(11,getmaxy()-24,getmaxx()-11,getmaxy()-11);
	setcolor(WHITE);
	outtextxy(20,getmaxy()-20,message);
}

// show_menu() shows the main menu on the screen and returns the option selected.
int show_menu()
{
	int ch,flag=0,j;
	char validation[20];

	gotoxy(15,5);
	printf("Automatic Time Table generator helps various educational ");
	gotoxy(7,6);
	printf("institutions in generating unique time-tables for different classes.");
	gotoxy(25,12);
	printf("1. Generate New Time-Table Project\n");
	gotoxy(25,14);
	printf("2. Load Saved Time-Tables Projects\n");
	gotoxy(25,16);
	printf("3. Exit\n");
	
	j=21;
	do
	{
		if(flag == 1)
			statusbar("Invalid input. Please re-enter.");
		gotoxy(24,j++);
		printf("Enter your choice: ");
		flushall();
		fgets(validation,sizeof validation,stdin); //gets the input from user from main menu.
	}while(flag=1,!sscanf(validation,"%d",&ch)); //validation
	
	return ch;
}

/* This function takes all the input required to create time-tables and store it in the structure
 * It returns back the structure with filled data.
 */
standard new_timetable()
{
	int i,flag=0,j;
	char *message,validation[20];
	
	standard std;
	
	//setting up default values for the structure variable
	std.nrecess = 0;
	std.yesno = 'y';
	
	
	outerbox();
	statusbar("Enter numeric digit 4 to 10.");
	flushall();
	i=3;
	do
	{
		do
		{
			if(flag == 1)
				statusbar("Invalid Input. Please enter numeric input between 4 and 10 only.");
			gotoxy(5,i++);
			printf("Enter total number of periods in a day : "); 
			flushall();
			fgets(validation,sizeof validation,stdin); //input

		}while(flag=1,!sscanf(validation,"%d", &std.nperiods_day)); //validation

	}while(std.nperiods_day>10 || std.nperiods_day<4); //validation

	
	flushall();
	j=12;
	for(i=0;i<std.nperiods_day;i++)
	{
		flag = 0;
		statusbar("Input a string which represents the time duration of period.");
		do
		{
			if(flag == 1)
				statusbar("Invalid input. Input cannot be an empty string.");
			gotoxy(5,j++);
			printf("Enter Time Duration for period %d : ",i+1);
			scanf("%[^\n]",std.periods[i]); //input
			flushall();
		}while(flag=1,strlen(std.periods[i])==0); //validation
	}

	outerbox();
	statusbar("Input numeric value between 0 to 2 or less than total periods.");
	i=3;
	flag=0;
	do
	{
		do
		{
			if(flag == 1)
				statusbar("Error. Input numeric value between 0 to 2 or less than total periods.");
			else if(flag == 2)
				statusbar("Error. If you have only 4 periods, you can only have at max 1 recess.");
			gotoxy(5,i++);
			printf("Enter total number of recess periods : ");
			flushall();
			fgets(validation,sizeof validation,stdin); //input
		}while(flag=1,!sscanf(validation,"%d", &std.nrecess)); //validation
	//validation rule: Total number of recess cannot be less than 0 or more than 2. It cannot be greater than total periods.
	}while(std.nrecess>2 || std.nrecess<0 || std.nrecess>std.nperiods_day || (std.nperiods_day<=4?(flag=2,std.nrecess>1):std.nrecess<0)); 

	if(std.nrecess!=0)
	{
		for(i=1;i<std.nperiods_day-1;i++)
		{
			gotoxy(33,9+i);
			//showing all possible recess periods. Ignoring the first and last one's.
			printf("%d => %s",i,std.periods[i]);
		}

		j=20;
		for(i=0;i<std.nrecess;i++)
		{
			flag=0;
			do
			{
				statusbar("Input the numeric code of the period.");
				do
				{

					if(flag == 1)
						statusbar("Error. Input appropriate period code.");
					else if(flag == 2)
						statusbar("Error. Choose other code. Selected code is already a recess period.");
					gotoxy(23,j++);
					printf("Enter Period Code for recess %d : ",i+1); 
					flushall();
					fgets(validation,sizeof validation,stdin); //input
				}while(flag=1,!sscanf(validation,"%d", &std.recess[i]));
				//validation rule: Recess period cannot be out of bound (from total periods). It cannot be an already selected recess. 
			}while(std.recess[i]<=0 || std.recess[i]>=std.nperiods_day-1 || (i!=0?(std.recess[i]==std.recess[i-1]?(flag=2,1):0):std.recess[i]==0));
		}
	}

	outerbox();
	statusbar("Enter numeric digit less than or equal to total number of periods.");
	flushall();
	i=3;
	flag=0;
	do
	{
		do
		{
			if(flag == 1)
				statusbar("Error. Please enter numeric digit less than or equal to total periods.");
			else if(flag == 2)
				statusbar("Error. Minimum number of subjects is 4. Maximum is total no of periods.");
			gotoxy(5,i++);
			printf("Enter total number of subjects: ");
			flushall();
			fgets(validation,sizeof validation,stdin); //input
		}while(flag=1,!sscanf(validation,"%d", &std.nsubjects));
		//validation rule : Subjects cannot be greater than total periods and cannot be less than 4.
	}while(flag=2,std.nsubjects>std.nperiods_day || std.nsubjects<4);

	
	flushall();
	j=12;
	for(i=0;i<std.nsubjects;i++)
	{
		flag = 0;
		statusbar("Input a string which represent the name of the subject.");
		do
		{
			if(flag == 1)
				statusbar("Invalid input. Input cannot be an empty string.");
			gotoxy(5,j++);
			printf("Enter Name of Subject %d : ",i+1);
			scanf("%[^\n]",std.subjects[i]); //input
			flushall();
		}while(flag=1,strlen(std.subjects[i])==0); //validation rule: input cannot be empty
	}


	outerbox();
	statusbar("Enter numeric digit less than or equal to total number of subjects.");
	flushall();
	i=3;
	flag=0;
	do
	{
		do
		{
			if(flag == 1)
				statusbar("Error. Please enter numeric digit less than or equal to total subjects.");
			else if(flag == 2)
				statusbar("You should have minimum one class or max classes equal to total subs - 1.");
			gotoxy(5,i++);
			printf("Enter total number of class divisions: ");
			flushall();
			fgets(validation,sizeof validation,stdin);
		}while(flag=1,!sscanf(validation,"%d", &std.nclasses));
		//validation rule: Total classes cannot be greater than total subjects minus 1. and it cannot be less than 1.
	}while(flag=2,std.nclasses>std.nsubjects-1 || std.nclasses<1);

	
	flushall();
	j=12;
	for(i=0;i<std.nclasses;i++)
	{
		flag = 0;
		statusbar("Input a string which represent the Division of the class.");
		do
		{
			if(flag == 1)
				statusbar("Invalid input. Input cannot be an empty string.");
			gotoxy(5,j++);
			printf("Enter Name of Class %d : ",i+1);
			scanf("%[^\n]",std.classes[i]);
			flushall();
		}while(flag=1,strlen(std.classes[i])==0);
	}

	//Only ask for "same subject allowed more than once in a day" input if Total subjects + Total Recess periods are less than Total Periods.
	if(std.nsubjects+std.nrecess<std.nperiods_day)
	{
	

		outerbox();
		statusbar("Input the character 'y' for Yes and 'n' for No.'");
		flushall();
		i=3;
		flag=0;
		do
		{
			if(flag == 1)
				statusbar("Error. Please input either 'y' for Yes or 'n' for No, only.");
			gotoxy(5,i++);
			printf("Is it allowed to have same subject more than once in a day? (y/n) : ");
			flushall();
			std.yesno = getche(); //input
		}while(flag=1,std.yesno != 'y' && std.yesno!='n'); //validation rule: It can only be 'y' or 'n'.
	}
	
	cleardevice();
	
	return std;
}

// It takes an structure variable as an input and generates an intermediate time-table which are processed later 
// by check_unique() function to make it unique.
standard generate_timetable(standard std)
{

	int i,j,k,m,scroll=0,scroll2;
	/* The below loop allocates an index value (which represents the subject string stored in an array)
	 * to each period on every day, sequentially. This generated time-table matrix is not unique.
	 * It is further processed to make it unique in check_unique() function.
	 */
	for(i=0;i<std.nclasses;i++)
	{
		for(k=0;k<6;k++)
		{
			for(j=0;j<std.nperiods_day;j++)
			{
				if(i==0)
				{
					//allocates the index
					std.timetable[i][j][k] = scroll;
					
					//reset the scroll back to -1 if total subjects - 1 is reached. it is allocated -1 because loop does ++ operation and makes it 0.
					if(scroll == std.nsubjects - 1)
					{
						scroll = -1;
					}
				}
				else
				{
					scroll2 = k+1;
					
					//reset scroll to 0 if it reaches 6 (6 here is total no of days in a week)
					if(scroll2 == 6)
						scroll2=0;
						
					std.timetable[i][j][k] = std.timetable[i-1][j][scroll2];
 				}	
 				scroll++;
			}
			//if Total periods divides Total subject is even. We need to scroll once more.
			if(std.nperiods_day%std.nsubjects==0)
			{
				scroll++;
				if(scroll == std.nsubjects - 1)
				{
					scroll = 0;
				}
			}
		}
	}

	for(i=0;i<std.nclasses;i++)
	{
		for(k=0;k<std.nsubjects;k++)
		{
			for(j=0;j<std.nperiods_day;j++,scroll++)
			{
				if(k>0)
 				{
 					for(m=1;m<=k;m++)
 					{
 						if(std.timetable[i][j][k] == std.timetable[i][j][k-m])
 						{
 							std.timetable[i][j][k]++;
 							if(std.timetable[i][j][k] == std.nsubjects )
 								std.timetable[i][j][k] = 0;
 						}
 					}
 				}

			}
		}
	}
	
	// copies the structure varaible to global_data (which is a global structure variable)
	global_data = std;
	
	check_unique();

	return global_data;


}

/* This function check if the time-table is unique or not.
 * It has no return type or parameters, to avoid stack overflow.
 * It manipulates on the globally declared structure variable global_data
 * It keeps on recurssing until all time-table for each class-division are unique.
 */
void check_unique()
{
	int i,j,k,m;
	
	//This loop scrolls through all time-tables of the project to check if they are unique or not. 
	//If they are not, make them unique and call this function again recursively until they become unique.
	for(i=1;i<global_data.nclasses;i++)
	{
		for(k=0;k<global_data.nsubjects;k++)
		{
			for(m=0;m<i;m++)
			{
				for(j=0;j<global_data.nperiods_day;j++)
				{
					//check if a period is same in all other time-tables, if yes. Incresses the index by 1.
					if(global_data.timetable[i][j][k] == global_data.timetable[m][j][k])
					{
						global_data.timetable[i][j][k]++;
						if(global_data.timetable[i][j][k] == global_data.nsubjects )
							global_data.timetable[i][j][k] = 0;
						//calls back the same function recursively until it becomes unique.
						check_unique();
					}
				}
			}
		}
	}
}


/* This function is used to draw the time-table on the screen and ask for various input to process further.
 * Paramters :
 * class_division : Index of the class for which time-table is displayed on the screen.
 * arrival_from : If its 0, it is draw just after new time table is generated. If it is 1, it is draw from loading an already saved project.
 * standard current : Passes the structure from which the data is taken to display the time-table.
 *
 * arrival_from is used to show appropriate operations. For example, it show show "save project" if its newly generated or it should show "delete project" if its loaded project.
 */
int draw_timetable(int class_division,int arrival_from,standard current)
{

	int i,j,k,m,index,o,period_scroll=0,left,maxlengthperiods,maxlengthsubjects,verticalspace=55,rectleft,recttop=55,flag=0,ch,n,indent,limit;
	char recess_string[7] = "RECESS",divisionheading[50],str[2],validation[20];
	char days[7][8]={"Periods","Mon","Tue","Wed","Thu","Fri","Sat"}; // Store the above heading of the time-tables


	//initiliaze maximum lenghts for period string and subjects string for deciding proper width of time-table
	if(higheststrlen(current.periods,current.nperiods_day)<textwidth(days[0]))
		maxlengthperiods = textwidth(days[0])+30;
	else
		maxlengthperiods = (higheststrlen(current.periods,current.nperiods_day))+30;

	if(higheststrlen(current.subjects,current.nsubjects)<textwidth(days[1]))
		maxlengthsubjects = textwidth(days[1])+30;
	else
		maxlengthsubjects = (higheststrlen(current.subjects,current.nsubjects))+30;

	//print time table division on top of each time table
	sprintf(divisionheading,"Time-Table For Class Division %s",current.classes[class_division]);
	outtextxy((getmaxx()-textwidth(divisionheading))/2,40,divisionheading);

	//decide the left axis of the rectangle for time-table
	left = (getmaxx()-(maxlengthperiods+(maxlengthsubjects*6)-30))/2;
	rectleft = left;

	//draw the bar for top-heading of the time table
	setfillstyle(SOLID_FILL,WHITE);
	bar(rectleft-10,recttop-3,getmaxx()-left+10,recttop+10);

	//print the heading of time-table
	setcolor(BLACK);
	for(o=0;o<7;o++)
	{
		if(o==0)
		{
			outtextxy(left,verticalspace,days[o]);
			left=left+maxlengthperiods;
		}
		else
		{
			outtextxy(left,verticalspace,days[o]);
			left=left+maxlengthsubjects;
		}
	}
	setcolor(WHITE);

	//loop to print the whole time-table on the screen.
	i = class_division;
	for(j=0;j<current.nperiods_day-current.nrecess;j++)
	{
		left = (getmaxx()-(maxlengthperiods+(maxlengthsubjects*6)-30))/2;

		outtextxy(left,verticalspace=verticalspace+20,current.periods[period_scroll++]);
		left+=maxlengthperiods;
		index=0;
		//If its an recess period, show RECESS
		if(current.nrecess==1?j==current.recess[0]:0 || current.nrecess>1?(j+1==current.recess[1] || j==current.recess[0]):0)
		{
			for(o=0;o<6;o++)
			{
				sprintf(str,"%c",recess_string[index++]);
				outtextxy(left,verticalspace,str);
				left=left+maxlengthsubjects;
			}

			left = (getmaxx()-(maxlengthperiods+(maxlengthsubjects*6)-30))/2;
			outtextxy(left,verticalspace=verticalspace+20,current.periods[period_scroll++]);
			left+=maxlengthperiods;
		}
		if(current.nsubjects>6)
			limit = 6;
		else
			limit = current.nsubjects;
			
		for(k=0;k<limit;k++)
		{
				if(current.yesno == 'n')
				{
					if(j>=current.nsubjects)
					{
						outtextxy(left,verticalspace,"Free");
						left=left+maxlengthsubjects;
					}
					else
					{
						outtextxy(left,verticalspace,current.subjects[current.timetable[i][j][k]]);
						left=left+maxlengthsubjects;
					}
				}
				else
				{
					outtextxy(left,verticalspace,current.subjects[current.timetable[i][j][k]]);
					left=left+maxlengthsubjects;
				}
		}
		
		if(limit !=6)
		{
			for(k=0;k<(6-limit);k++)
			{
				if(current.yesno == 'n')
				{
					if(j>=current.nsubjects)
					{
						outtextxy(left,verticalspace,"Free");
						left=left+maxlengthsubjects;
					}
					else
					{
						outtextxy(left,verticalspace,current.subjects[current.timetable[i][j][k]]);
						left=left+maxlengthsubjects;
					}
				}
				else
				{
					outtextxy(left,verticalspace,current.subjects[current.timetable[i][j][k]]);
					left=left+maxlengthsubjects;
				}
			}
		}
	}
	
	//Draws the rectangle around the time-table
	rectangle(rectleft-10,recttop-3,left-20,verticalspace+=20);

	//shows appropriate menu options according to the execution of this function.
	// It shows delete project, if it is showing an existing project or else it shows Save project option to save the project to our data file for future reference.
	setfillstyle(SOLID_FILL,WHITE);
	if(class_division==current.nclasses-1)
	{
		indent = ((getmaxx()-textwidth("1.Delete Project  2.Back to Menu  3.Export HTML"))/2)-10;
		bar(indent,verticalspace+=15,textwidth("1.Delete Project  2.Back to Menu  3.Export HTML")+20+indent,verticalspace+=15);
		setcolor(BLACK);
		if(arrival_from==1)
			outtextxy((getmaxx()-textwidth("1.Delete Project  2.Back to Menu  3.Export HTML"))/2,verticalspace-10,"1.Delete Project  2.Back to Menu  3.Export HTML");
		else
			outtextxy((getmaxx()-textwidth("1.Save Project  2.Back to Menu  3.Export HTML"))/2,verticalspace-10,"1.Save Project  2.Back to Menu  3.Export HTML");
		setcolor(WHITE);
	}
	else
	{
		indent = ((getmaxx()-textwidth("1.Delete Project  2.Back to Menu  3.Export HTML  4.Next Table"))/2)-10;
		bar(indent,verticalspace+=15,textwidth("1.Delete Project  2.Back to Menu  3.Export HTML  4.Next Table")+20+indent,verticalspace+=15);
		setcolor(BLACK);
		if(arrival_from==1)
			outtextxy((getmaxx()-textwidth("1.Delete Project  2.Back to Menu  3.Export HTML  4.Next Table"))/2,verticalspace-10,"1.Delete Project  2.Back to Menu  3.Export HTML  4.Next Table");
		else
			outtextxy((getmaxx()-textwidth("1.Save Project  2.Back to Menu  3.Export HTML  4.Next Table"))/2,verticalspace-10,"1.Save Project  2.Back to Menu  3.Export HTML  4.Next Table");
		setcolor(WHITE);
	}

	//Adjust the location to ask the input from
	n=current.nperiods_day*3;
	if(n>20)
		n=21;
	else if(n<=12)
		n+=2;

	//ask the input to select time-table options
	do
	{
		if(flag == 1)
			statusbar("Invalid input. Please re-enter.");
		gotoxy(30,n++);
		printf("Enter your choice: ");
		flushall();
		fgets(validation,sizeof validation,stdin);
	}while(flag=1,!sscanf(validation,"%d",&ch)); //validation
	
	return ch; //return the selected option
}

/* This function is used to return an integer value which represents the highest length from an array of strings.
 * Paramters:
 * inputarray - Multi-dimensional character array to check highest length from.
 * arraysize - Total number of strings within the mult-dimensional array.
 *
 * This function is used within draw_timetable() to dynamically draw timetable on the screen, according to the string length of inputs.
 */
int higheststrlen(char inputarray[10][20], int arraysize)
{
	int highestcount,i;
	highestcount = textwidth(inputarray[0]);

	for(i=1;i<arraysize;i++)
	{
		//if length of array element is greater than previous count, call it highest.
		if(strlen(inputarray[i]) > highestcount)
			highestcount = textwidth(inputarray[i]);
	}

	return highestcount;
}

/* This function saved the project in the binary file.
 * It takes one parameter, which is standard std, it is the structure variable which contains all the data to be saved.
 *
 * This function is called from draw_timetable() when appropriate option is selected to save the generated time-table project.
 * It asks users a name for the project. This is the project_name variable from the structure.
 */
void save_project(standard std)
{
	int a=5,flag=0;
	FILE *fp;
	
	outerbox();
	statusbar("Give a name to this project.");

	do
	{
		if(flag == 1)
			statusbar("Invalid input. Input cannot be an empty string.");
		gotoxy(5,a++);
		printf("Enter Name for this project: ");

		scanf("%[^\n]",std.projectname);
		flushall();
		//validation : Input cannot be empty string.
	}while(flag=1,strlen(std.projectname)==0);

	//PRJV_DB.TXT is the data file of the project. It stores all the data of every project.
	fp = fopen("PRJV_DB.TXT","ab");
	
	fwrite(&std,sizeof std,1,fp); //writes the project (structure variable) to the data file
	
	fclose(fp);
	
	//show message on center of the screen.
	outtextxy((getmaxx()-textwidth("Project Saved Succesfully. Press any key to move to main menu."))/2,300,"Project Saved Succesfully. Press any key to move to main menu.");
	getch();

}

/* This function reads the binary file and lists all the project names on the screen.
 * User is then asked to select an project to be loaded.
 * Entered index is returned from the function, which is further used by load_project() to laod the project.
 */
int show_projects()
{
	int p=1,o=30,flag=0,ch;
	char buffer[4],validation[20];
	FILE *fp;
	standard show;
	

	outerbox();
	statusbar("Please select the project you want to load.");
	
	fp = fopen("PRJV_DB.TXT","rb"); //open data file
	
	if(fp)
	{
		fseek(fp,0,SEEK_END); //move to the end of the file
		if(ftell(fp) == 0) // if file is empty, show the no projects message.
		{
			fclose(fp);
			gotoxy(14,20);
			printf("No Saved Projects. Press any key to move back to menu.");
			getch();
			return 0;
		}
	}
	else // or else, if the file does not exist, show the no projects message.
	{
		fclose(fp);
		gotoxy(14,20);
		printf("No Saved Projects. Press any key to move back to menu.");
		getch();
		return 0;
	}
	
	rewind(fp); //move to the starting of the data file.
	
	while(!feof(fp))
	{
		fread(&show,sizeof show,1,fp); //read data
		if(!feof(fp))
		{
			//print all the project names on the screen
			sprintf(buffer,"%d",p);
			outtextxy(250,o+=20,buffer);
			outtextxy(280,o,show.projectname);
			p++;
		}
	}
	
	//show the back to menu option on the screen.
	sprintf(buffer,"%d",0);
	outtextxy(210,o+=40,buffer);
	outtextxy(240,o,"Back to Main Menu");
	
	fclose(fp); //close data file
	
	o=22;
	do
	{
		if(flag == 1)
			statusbar("Invalid input. Please re-enter.");
		gotoxy(30,o++);
		printf("Enter your choice: ");
		flushall();
		fgets(validation,sizeof validation,stdin);
	}while(flag=1,!sscanf(validation,"%d",&ch)); //validation
	
	//choice cannot be less than 0 or greater than total no of projects
	if(ch>=p || ch<0)
	{
		outerbox();
		statusbar("Invalid option selected. Please re-select.");
		show_projects();
	}

	outerbox();
	statusbar("Project Loaded Succesfully.");
	
	return ch;
}

/* This function is used to load the project data into an structure variable.
 * It takes project_id as an input.
 * It returns back the structrue variable with data of loaded project.
 */
standard load_project(int project_id)
{
	int p=1;
	FILE *fp;
	standard load;
	
	fp = fopen("PRJV_DB.TXT","rb"); //open data file
	
	while(!feof(fp))
	{
		fread(&load,sizeof load,1,fp); //read data
		if(!feof(fp))
		{
			if(project_id == p) // if project id is found, break the loop.
				break;
			
			p++;
		}
	}
	
	fclose(fp); //closes data file
	
	//returns the loaded project structure variable
	return load;
}

/* This function is used to delete an project.
 * It takes project_id and deletes the record of that project from the binary file
 */
void delete_project(int project_id)
{
	int p=1;
	FILE *fp,*fp2;
	standard get,set;
	
	fp = fopen("PRJV_DB.TXT","rb"); //open main data file
	fp2 = fopen("PRJV_TMP.TXT","wb"); //open temporary file to write data
	
	while(!feof(fp))
	{
		fread(&get,sizeof get,1,fp); //read data from main data file
		if(!feof(fp))
		{
			if(project_id != p) //write to the temporary file if the project id is NOT the one to be deleted.
			{
				fwrite(&get,sizeof get,1,fp2);
			}
			p++;
		}
			
	}
	
	fclose(fp2); //close temporary data file
	fclose(fp); //close main data file
	
	fp = fopen("PRJV_DB.TXT","wb"); //open main data file
	fp2 = fopen("PRJV_TMP.TXT","rb"); // open temporary data file
	
	while(!feof(fp2))
	{
		fread(&set,sizeof set,1,fp2); //read data
		if(!feof(fp2))
		{
			fwrite(&set,sizeof set,1,fp); //rewrite data from temporary data file to main data file.
		}
	}
	
	fclose(fp2); //closes temporary data file
	fclose(fp); //closes main data file
	
	unlink("PRJV_TMP.TXT"); //delete the temporary data file
	
	outerbox();
	statusbar("Press any key to move back to main menu");
	
	//show the message on center of screen
	outtextxy((getmaxx()-textwidth("Project Deleted Succesfully. Press any key to move to main menu."))/2,300,"Project Deleted Succesfully. Press any key to move to main menu.");
	getch();
	
}

/* This function generates the HTML text for the project, which is saved into an HTML file.
 * Paramters:
 * project_id : It takes the project_id as an argument to generate HTML text for that project.
 * ... : As ANSI C does not allow optional paramters (as in c++), we here use argument lists.
 * Arguments lists can catch up passed arguments and process them using functions like va_start, va_end, etc.
 * STDARG.H header file is included inorder to use this functionality.
 * 
 * Sometimes when the HTML is to be generated for an project without saving the project, the structure is directly passed and project_id is kept 0.
 * Passed structure is caught using va_arg
 *
 * This function asks for the name of the HTML file to save the data. The name should not be more than 8 characters long.
 * The HTML file generated shows all the time-tables for each class-division.
 * There is a quick link to print the whole page, so that print out can be taken and pasted on notice boards within the institutes.
 */
void export_html(int project_id, ...)
{
	int p=1,flag=0,a=5,i,j,k,o,limit,scroll=0;
	FILE *fp,*html;
	char html_filename[20],recess_string[7] = "RECESS";
	standard for_html;
	
	
	va_list ap;
	va_start(ap,project_id);
	for_html = va_arg(ap,standard);
	va_end(ap);
	
	outerbox();
	statusbar("Enter a filename for the HTML file. Should not be more than 8 characters long.");
	
	
	if(project_id != 0)
	{
		fp = fopen("PRJV_DB.TXT","rb");
	
		while(!feof(fp))
		{
			fread(&for_html,sizeof for_html,1,fp);
			if(!feof(fp))
			{
				if(project_id == p)
					break;
				p++;
			}
		}
		fclose(fp);
	}
	
	do
	{
		if(flag == 1)
			statusbar("Input cannot be an empty string or greater than 8 characters");
		gotoxy(5,a++);
		printf("Enter a name for HTML File:  ");

		scanf("%[^\n]",html_filename);
		flushall();
		//validation rule : input cannot be empty. Input cannot be more tha 8 characters long.
	}while(flag=1,strlen(html_filename)==0 || strlen(html_filename)>8);
	
	//replace space with _ in filename 
	for(i=0;i<strlen(html_filename);i++)
	{
		if(html_filename[i]==' ')
			html_filename[i]='_';
	}
	
	//add .htm extension to file name. so that we can save it as HTML file
	strcat(html_filename,".htm");
	
	html = fopen(html_filename,"w"); //write to the html file
	
	
	if(!html)
		perror("error : ");
	//Write the formatted data to the html file. 
	else
	{
		//show heading and print this page option
		fprintf(html,"%s","<html><head><title>Automatic Time-Table Generator v1.0</title></head><body><center><a href='#' onclick='window.print()'>Click here to print this page</a>");

		for(i=0;i<for_html.nclasses;i++)
		{
			fprintf(html,"%s","<h3>Time-Table for Division: ");
			fprintf(html,"%s",for_html.classes[i]);
			fprintf(html,"%s","</h3>");
			
			//generate html code for the time-tables
			fprintf(html,"%s","<table border='1' style='text-align:center;' cellpadding='5'><tr><td><b>Periods</b></td><td><b>Mon</b></td><td><b>Tue</b></td><td><b>Wed</b></td><td><b>Thu</b></td><td><b>Fri</b></td><td><b>Sat</b></td></tr>");
			scroll = 0;
			for(j=0;j<for_html.nperiods_day-for_html.nrecess;j++)
			{
				
				fprintf(html,"%s","<tr><td><b>");
				fprintf(html,"%s",for_html.periods[scroll++]);
				fprintf(html,"%s","</b></td>");
				
				if(for_html.nrecess==1?j==for_html.recess[0]:0 || for_html.nrecess>1?(j+1==for_html.recess[1] || j==for_html.recess[0]):0)
				{
					for(o=0;o<6;o++)
					{
						fprintf(html,"%s","<td>");
						fprintf(html,"%c",recess_string[o]);
						fprintf(html,"%s","</td>");
					}
					fprintf(html,"%s","</tr><tr><td><b>");
					fprintf(html,"%s",for_html.periods[scroll++]);
					fprintf(html,"%s","</b></td>");
					
				}
				
				if(for_html.nsubjects>6)
					limit = 6;
				else
					limit = for_html.nsubjects;
					
				for(k=0;k<limit;k++)
				{
						if(for_html.yesno == 'n')
						{
							if(j>=for_html.nsubjects)
							{
								fprintf(html,"%s","<td>Free</td>");
							}
							else
							{
								fprintf(html,"%s %s %s","<td>",for_html.subjects[for_html.timetable[i][j][k]],"</td>");
							}
						}
						else
						{
							fprintf(html,"%s %s %s","<td>",for_html.subjects[for_html.timetable[i][j][k]],"</td>");
						}
				}
		
				if(limit != 6)
				{
					for(k=0;k<(6-limit);k++)
					{
							if(for_html.yesno == 'n')
							{
								if(j>=for_html.nsubjects)
								{
									fprintf(html,"%s","<td>Free</td>");
								}
								else
								{
									fprintf(html,"%s %s %s","<td>",for_html.subjects[for_html.timetable[i][j][k]],"</td>");
								}
							}
							else
							{
								fprintf(html,"%s %s %s","<td>",for_html.subjects[for_html.timetable[i][j][k]],"</td>");
							}
					}
				}

				fprintf(html,"%s","</tr>");
			}
			fprintf(html,"%s","</table><br/>");
		}
		fprintf(html,"%s","</center></body><html>");
	}
	
	fclose(html); //close the html file
	
	//show proper message on center of the screen.
	outtextxy((getmaxx()-textwidth("Project Sucesfully Exported to the HTML File. Press any key to continue."))/2,300,"Project Sucesfully Exported to the HTML File. Press any key to continue.");
	getch();
	
}
