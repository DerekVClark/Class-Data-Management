#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"
#include <unordered_map>

using namespace std;


// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prereqs;
};

//Load courses from csv path
pair<unordered_map<string, Course>, vector<Course>> loadCourses(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;
	// Create unordered map courses with string key and Course value
    unordered_map<string, Course> coursesMap;
	//create vector courses with Course value
	vector<Course> coursesVector;


    csv::Parser file = csv::Parser(csvPath);

    try {
        //for each row
        for (int i = 0; i < file.rowCount(); i++) {
            //create course
            Course course;
            //set course id and title to item 0 and 1 of line
            course.courseId = file[i][0];
            course.title = file[i][1];
            //loop through prereqs adding to course
            for (int j = 2; j < file[i].size(); j++) {
                //if cell is not empty in csv
                if (!file[i][j].empty()) {
                    // Push prereq courses into vector
                    course.prereqs.push_back(file[i][j]);

                }
            }
            // Adds course to map using courseId as key
            coursesMap[course.courseId] = course;
            // Adds course to vector
            coursesVector.push_back(course);
        }
    }
    // Catches errors
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
	}
	// Catches any other exceptions
	catch (const exception& e) {
		std::cerr << "Unexpected error: " << e.what() << std::endl;
	}
    // Return map and vector
    return { coursesMap, coursesVector };
}

void displayCourse(Course course) {
	// Prints course id and title
    cout << course.courseId << ": " << course.title;
	// If prereqs exist
    if (course.prereqs.size() > 0) {
        cout << " | ";
        //loop prints prereqs if exists
        for (const auto& prereq : course.prereqs) {
            cout << prereq << " ";
        }
    }
    cout << endl;
}

void getCourse(unordered_map<string, Course>& coursesMap, string toFind) {
	auto it = coursesMap.find(toFind);
	if (it != coursesMap.end()) {
		displayCourse(it->second);
	}
	else {
		cout << "Course not found" << endl;
	}
}

int partition(vector<Course>& courses, int begin, int end) {
    //set low and high equal to begin and end
    int low = begin;
    int high = end;

    // pick the middle element as pivot point
    int pivot = begin + (end - begin) / 2;
    //Not completed
    bool complete = false;

    // while not done 
    while (!complete) {
        // keep incrementing low index while courses[low] < courses[pivot]
        while (courses[low].title.compare(courses[pivot].title) < 0) {
            ++low;
        }
        // keep decrementing high index while courses[pivot] < courses[high]
        while (courses[pivot].title.compare(courses[high].title) < 0) {
            --high;
        }
        /* If there are zero or one elements remaining,
            all courses are partitioned. Return high */
        if (low >= high) {
            complete = true;
        }
        // else swap the low and high course (built in vector method)
        else {
            swap(courses[low], courses[high]);
            // move low and high closer ++low, --high
            ++low;
            --high;
        }
    }

    return high;
}

void quickSort(vector<Course>& courses, int begin, int end) {
    //set mid equal to 0
    int mid = 0;

    /* Base case: If there are 1 or zero bids to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/
    if (begin >= end) {
        return;
    }

    /* Partition bids into low and high such that
     midpoint is location of last element in low */
    mid = partition(courses, begin, end);



    // recursively sort low partition (begin to mid)
    quickSort(courses, begin, mid);

    // recursively sort high partition (mid+1 to end)
    quickSort(courses, mid + 1, end);

}

int main(int argc, char* argv[]) {
    string csvPath;

    unordered_map<string, Course> coursesMap;
	vector<Course> coursesVector;

    clock_t ticks;
    //intialize user choice
    int choice = 0;
    //initializes course to be found
    string toFind;
    //Initializes loaded as false untill courses have been loaded
    bool loaded = false;
    while (choice != 9) {
        cout << "Menu" << endl;
        cout << " 1. Load Courses" << endl;
        cout << " 2. Display Courses" << endl;
        cout << " 3. Find Course by ID" << endl;
        cout << " 9. Exit" << endl;
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
         //Loads csv courses into vector
        case 1:
			// If courses are already loaded
            if (loaded) {
				cout << "Courses already loaded" << endl;
				break;
            }
            // While the file did not load
            while (!loaded) {
                cout << "Please provide the csv file: ";
                cin >> csvPath;
                // Tries to get course data
                try {
					// Load courses and obtains course map and vector
                    tie(coursesMap, coursesVector) = loadCourses(csvPath);

                    cout << coursesMap.size() << " Courses succesfully read" << endl;

                    loaded = true;
                }
                // If course data was not successfully loaded
                catch (const csv::Error& e) {
                    cerr << e.what() << endl;
                    cout << "Failed to load data from csv file please try again..." << endl;
                }
                catch (const exception& e) {
                    cerr << "Unexpected error: " << e.what() << endl;
                }
                
                
            }
            break;
        // Displays courses if loaded into vector
        case 2:
            if(loaded == true){
                quickSort(coursesVector, 0, coursesVector.size() - 1);
                for (int i = 0; i < coursesVector.size(); i++) {
                    displayCourse(coursesVector[i]);
                }
                cout << endl;
            }
            //if not loaded tells user to load csv
            else {
                cout << "You must first load the courses" << endl;
            }
            

            break;
        //Finding a course by its coursId
        case 3:
            cout << "Please provide the course ID: ";
            cin >> toFind;
            ticks = clock();
            getCourse(coursesMap, toFind);
            
            cout << coursesMap.size() << " Courses read" << endl;
            
            break;
        //Deals with any other option not currently in switch
        default:
            cout << choice << " Is not a valid selection";
        }
    }
    cout << "Goodbye." << endl;

    return 0;
}