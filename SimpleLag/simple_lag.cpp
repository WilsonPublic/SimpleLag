/*

	SimpleLag, by Wilson

*/

#include <windows.h> 
#include <string>
#include <thread>

// looks nice, ok...
#define sleep(_time) (std::this_thread::sleep_for(std::chrono::milliseconds(_time))) 

// windows firewall rule name and game executable path
std::string rule_name = "simplelag";
std::string executable_path = "C:\\Battlestate Games\\EFT (live)\\EscapeFromTarkov.exe";

// defined keys
int lag_key = VK_F2;
int exit_key = VK_END;

// lag time in ms
int lag_time = 3500;

int main() {
	// hide window
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	// self explanitory
	MessageBox(NULL, "SimpleLag is running in the background", "SimpleLag", MB_OK);

	std::string cmd = "netsh advfirewall firewall delete rule name=\"" + rule_name + "\"";
	WinExec(cmd.c_str(), 0);

	while (!(GetAsyncKeyState(exit_key) & 0x8000)) { // loop until exit key is pressed
		if (GetAsyncKeyState(lag_key) & 0x8000) {
			// play system beep sound to notify users that they're lagging
			Beep(800, 200);

			// create rule to block outbound
			cmd = "netsh advfirewall firewall add rule name =\"" + rule_name + "\" dir=out action=block program=\"" + executable_path + "\"";
			WinExec(cmd.c_str(), 0);

			// create rule to block inbound
			cmd = "netsh advfirewall firewall add rule name =\"" + rule_name + "\" dir=in action=block program=\"" + executable_path + "\"";
			WinExec(cmd.c_str(), 0);

			// wait for set time (adapt this yourself to incorporate a cancel key)
			sleep(lag_time);

			// delete both outbound and inbound rules
			cmd = "netsh advfirewall firewall delete rule name=\"" + rule_name + "\"";
			WinExec(cmd.c_str(), 0);

			// play system beep sound to notify users that they've stopped lagging
			Beep(500, 200);
		}
		sleep(5); // avoid high cpu usage
	}
	
	// play system beep sound to notify users that they've exited
	Beep(200, 200);

	return 0;
}
