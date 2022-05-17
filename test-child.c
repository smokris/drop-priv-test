#include <stdio.h>
#include <unistd.h>
#include <sys/capability.h>

int main(void)
{
	printf(" test-child: uid=%d euid=%d gid=%d egid=%d\n", getuid(), geteuid(), getgid(), getegid());

	puts(" test-child: dropping root privileges...");
	if (setgid(89))
		perror(" test-child:     setgid failed");
	if (setuid(89))
		perror(" test-child:     setuid failed");

	cap_t caps = cap_init();
	cap_set_proc(caps);
	cap_free(caps);

	printf(" test-child: uid=%d euid=%d gid=%d egid=%d\n", getuid(), geteuid(), getgid(), getegid());

	puts(" test-child: attempting to regain root privileges...");

	static cap_value_t suidcaps[] = {
		CAP_CHOWN,
		CAP_KILL,
		CAP_SYS_CHROOT,
		CAP_SETUID,
		CAP_SETGID,
		CAP_NET_BIND_SERVICE,
		CAP_DAC_OVERRIDE
	};
	caps = cap_init();
	cap_set_flag(caps, CAP_PERMITTED, sizeof(suidcaps)/sizeof(cap_value_t), suidcaps, CAP_SET);
	cap_set_flag(caps, CAP_EFFECTIVE, sizeof(suidcaps)/sizeof(cap_value_t), suidcaps, CAP_SET);
	cap_set_proc(caps);
	cap_free(caps);

	if (setgid(0))
		perror(" test-child:     setgid failed");
	if (setuid(0))
		perror(" test-child:     setuid failed");

	printf(" test-child: uid=%d euid=%d gid=%d egid=%d\n", getuid(), geteuid(), getgid(), getegid());

	return 0;
}
