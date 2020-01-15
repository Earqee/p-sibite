#include <bits/stdc++.h>

class Task
{
public:
	std::string title;
	Task(std::string name){
		title = name;
	}
	void printTask(){
		printf("%s\n", title.c_str());
	}	
};
class Week
{
public:
	std::vector<std::vector<Task>> days;
	Week(){}
	void addAtask(std::string title, int day)
	{	
		Task task(title);
		days[day].push_back(task);
	}
	void removeAtask(int day, int index)
	{
		days[day].erase(days[day].begin()+index);
	}
	void editAtask(int day, int index, std::string title)
	{
		days[day][index].title = title;
	}
	void printAday(int day)
	{
		for (int i = 0; i < days[day].size(); ++i)
		{
			printf("Task [%d]: ", i);
			days[day][i].printTask();
		}
	}
	void printAweek()
	{
		for (int i = 0; i < 7; ++i)
		{	
			printf("Day [%d]\n", i);
			printAday(i);
		}
	}
	
};
class OrganizerData
{
public:
	Week week;
	OrganizerData()
	{
		week = Week();
	}
	void initialMenu()
	{
		printf("Welcome\n");
		printf("What you want to do?\nAdd a Task [1]		Remove a Task [2]		Edit a Task [3]		View a day [4]		View a week [5]\n");
	}
	
};
int main()
{
	OrganizerData user;
	user.initialMenu();
	return 0;
}