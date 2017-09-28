/*vim: set ts=8 sw=4 tw=0 noet :
 *@file     expiretimer.cpp
 *@brief    simple countdown timer, which shows a notification after a given
 *          time
 *@author   bdk
 *@version  0.2
 *
 * version --  change -- author
 * 0.1 -- add first implentation, basic functions   -- bdk
 * 0.2 -- add verbose flag (-d) i                   -- bdk
 */

#include <stdio.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <thread>

#include <libnotifymm.h>
#include <tclap/CmdLine.h>

int main(int argc, char **argv)
{
    try {
        TCLAP::CmdLine cmd("Popup timer", ' ', "0.2");
        TCLAP::ValueArg<std::string> msgArg(
            "m", "msg", "Message to display in the notification", false,
            "Default message", "string");
        cmd.add(msgArg);

        TCLAP::UnlabeledValueArg<int> expiretime(
            "time", "expire time in minutes", true, 12, "int");
        cmd.add(expiretime);

        TCLAP::SwitchArg verboseFlag("d", "debug",
                                     "Enable verbose/debug output", cmd, false);

        cmd.parse(argc, argv);

        bool verbose = verboseFlag.getValue();

        pid_t pid = fork();

        if (pid == 0) {  // child
            std::this_thread::sleep_for(
                std::chrono::minutes(expiretime.getValue()));

            Notify::init("Popup timer");
            Notify::Notification popup("Popup timer expired", msgArg.getValue(),
                                       "dialog-warning");
            popup.set_timeout(0);
            popup.show();

            return EXIT_SUCCESS;
        }
        else if (pid > 0) {  // parent
            if (verbose) {
                std::cout << argv[0] << ": Starting Timer " << msgArg.getValue()
                          << " with " << expiretime.getValue() << " min"
                          << std::endl;
            }
            return EXIT_SUCCESS;
        }
        else {
            std::cerr << "Fork failed " << std::endl;
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    catch (TCLAP::ArgException &e) {
        std::cerr << "error: " << e.error() << " for arg " << e.argId()
                  << std::endl;
        return EXIT_FAILURE;
    }
}
