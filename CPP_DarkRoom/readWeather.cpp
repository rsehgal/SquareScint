#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
int main() {
    std::ifstream file("weather_log.csv");
    if (!file.is_open()) return -1;

    std::vector<int> timestamps;

    std::vector<double> pressureVec;
    
    // Optimization: Reserve memory if you know the approximate number of rows
    // e.g., for 72 hours at 1Hz, that's ~260,000 rows.
    //timestamps.reserve(300000); 

    std::string dummy;
    double ts;
    double ps;

    // Use a while loop with getline to handle the comma-separated format
    while (std::getline(file, dummy, ',')) { // Read Date up to the first comma
        if (file >> ts) { // Read the Unix timestamp
            timestamps.push_back(ts);
            file>>ps;
	    pressureVec.push_back(ps);
        }
        
        // Skip the rest of the line (Temp and Pressure)
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Successfully loaded " << timestamps.size() << " timestamps." << std::endl;

    std::for_each(pressureVec.begin(),pressureVec.end(),[](const auto val){std::cout << val << std::endl;});


    std::vector<double> hourly_averages;

   /* auto current_pos = unix_times.begin();
    while (current_pos != unix_times.end()) {
        // We pass 'current_pos' by reference; the lambda will move it for us
        double m = MuonPhys::get_next_bin_mean(current_pos, unix_times.end(), 3600.0);
        
        if (m > 0) {
            hourly_averages.push_back(m);
        }
    }*/
    return 0;
}
