#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/capability.h>

void forkChild(void)
{
	pid_t p = fork();
	if (p)
		waitpid(p, NULL, 0);
	else
		execl("./test-child", "test-child", NULL);
}

int main(void)
{
	puts("test-parent: ========= no cap_set_proc");
	forkChild();

	puts("test-parent: ========= cap_set_proc with Permitted and Effective");
	static cap_value_t suidcaps[] = {
		CAP_CHOWN,
		CAP_KILL,
		CAP_SYS_CHROOT,
		CAP_SETUID,
		CAP_SETGID,
		CAP_NET_BIND_SERVICE,
		CAP_DAC_OVERRIDE
	};
	cap_t caps = cap_init();
	cap_set_flag(caps, CAP_PERMITTED, sizeof(suidcaps)/sizeof(cap_value_t), suidcaps, CAP_SET);
	cap_set_flag(caps, CAP_EFFECTIVE, sizeof(suidcaps)/sizeof(cap_value_t), suidcaps, CAP_SET);
	cap_set_proc(caps);

	forkChild();

	puts("test-parent: ========= cap_set_proc with Permitted and Effective and Inheritable");
	cap_set_flag(caps, CAP_INHERITABLE, sizeof(suidcaps)/sizeof(cap_value_t), suidcaps, CAP_SET);
	cap_set_proc(caps);

	forkChild();

	cap_free(caps);

	return 0;
}
