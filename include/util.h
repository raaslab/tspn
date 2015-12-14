#ifndef __UTIL_H
#define __UTIL_H

/**
 * Misc. utility functions
 */

// Fancy display progress
// from http://stackoverflow.com/questions/60221/how-to-animate-the-command-line
void drawProgressBar(int len, double percent) {
	cout << "\x1B[2K"; // Erase the entire current line.
	cout << "\x1B[0E"; // Move to the beginning of the current line.
	string progress;
	for (int i = 0; i < len; ++i) {
		if (i < static_cast<int>(len * percent)) {
			progress += "=";
		} else {
			progress += " ";
		}
	}
	cout << "[" << progress << "] " << (static_cast<int>(100 * percent)) << "%";
	flush(cout); // Required.
}

// Just implements float to doubles
string ftoa(double value) {
	std::ostringstream ss;
	if (!(ss << value))
		return "";
	return ss.str();
}

// compute value of nchoosek
double nchoosek(int n, int kk) {
	double val = 1;
	double k = kk;
	assert(n>=k);
	if(k > n/2)	k = n-k;

	while(k > 0) {
		val = val*(n-k+1);
		val = val/(kk-k+1);
		k = k - 1;
	}
	return val;
}


#endif /* __UTIL_H */
