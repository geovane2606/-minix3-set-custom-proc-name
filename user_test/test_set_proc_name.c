/*
 * Programa de teste em User-Space para a syscall set_custom_proc_name(pid, name)
 * Dupla #5: Geovane e Sayan - MINIX 3.3.0
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Le o nome atual de um processo diretamente via /proc/<pid>/psinfo */
static int read_proc_name(pid_t pid, char *out, size_t out_size)
{
	char path[64];
	FILE *fp;
	int version, endpt;
	char type;
	char pname[64];

	snprintf(path, sizeof(path), "/proc/%d/psinfo", (int) pid);
	fp = fopen(path, "r");
	if (fp == NULL)
		return -1;

	/* Formato do psinfo no MINIX 3.3.0:
	 * <version> <type> <endpoint> <name> ...
	 */
	if (fscanf(fp, "%d %c %d %63s", &version, &type, &endpt, pname) != 4) {
		fclose(fp);
		return -1;
	}
	fclose(fp);

	strncpy(out, pname, out_size - 1);
	out[out_size - 1] = '\0';
	return 0;
}

static void print_proc_name(const char *label, pid_t pid)
{
	char name[64];

	if (read_proc_name(pid, name, sizeof(name)) == 0)
		printf("  [%s] PID %d -> nome atual no sistema: '%s'\n",
		    label, (int) pid, name);
	else
		printf("  [%s] PID %d -> (nao foi possivel ler /proc/%d/psinfo)\n",
		    label, (int) pid, (int) pid);
}

int main(int argc, char *argv[])
{
	pid_t target_pid;
	pid_t child_pid;
	int ret;

	printf("============================================================\n");
	printf(" Teste da Syscall set_custom_proc_name(pid, name)\n");
	printf(" Dupla #5: Geovane e Sayan (MINIX 3.3.0)\n");
	printf("============================================================\n\n");

	/* Modo 1: argumentos passados na linha de comando (<pid> <novo_nome>) */
	if (argc == 3) {
		target_pid = (pid_t) atoi(argv[1]);
		printf("[Modo Manual] Alterando PID %d para '%s'...\n",
		    (int) target_pid, argv[2]);
		print_proc_name("ANTES", target_pid);

		ret = set_custom_proc_name(target_pid, argv[2]);
		if (ret != 0) {
			printf("  ERRO: set_custom_proc_name retornou %d (errno=%d: %s)\n",
			    ret, errno, strerror(errno));
			return 1;
		}

		printf("  SUCESSO: set_custom_proc_name retornou 0!\n");
		/* Aguarda o procfs atualizar o cache interno (last_update != now) */
		sleep(1);
		print_proc_name("DEPOIS", target_pid);
		return 0;
	}

	/* Modo 2: bateria completa automatica */
	/* Teste 1: Alterar o nome do proprio processo */
	target_pid = getpid();
	printf("1) Teste no proprio processo (PID %d):\n", (int) target_pid);
	print_proc_name("ANTES", target_pid);

	ret = set_custom_proc_name(target_pid, "geovane_sayan");
	if (ret != 0) {
		printf("  ERRO inesperado: ret=%d, errno=%d (%s)\n",
		    ret, errno, strerror(errno));
		return 1;
	}
	printf("  set_custom_proc_name(%d, \"geovane_sayan\") -> retorno = %d (OK)\n",
	    (int) target_pid, ret);
	/* Aguarda o procfs atualizar o cache interno (last_update != now) */
	sleep(1);
	print_proc_name("DEPOIS", target_pid);
	printf("\n");

	/* Teste 2: Alterar o nome de um processo filho (fork) */
	printf("2) Teste alterando o nome de um processo filho (fork):\n");
	child_pid = fork();
	if (child_pid < 0) {
		perror("fork");
		return 1;
	}
	if (child_pid == 0) {
		/* Processo filho apenas aguarda alguns segundos */
		sleep(5);
		_exit(0);
	}

	/* Aguarda o procfs reconstruir os diretorios de PID em /proc */
	sleep(1);
	print_proc_name("FILHO ANTES", child_pid);
	ret = set_custom_proc_name(child_pid, "proc_filho_ok");
	if (ret != 0) {
		printf("  ERRO inesperado no filho: ret=%d, errno=%d (%s)\n",
		    ret, errno, strerror(errno));
		waitpid(child_pid, NULL, 0);
		return 1;
	}
	printf("  set_custom_proc_name(%d, \"proc_filho_ok\") -> retorno = %d (OK)\n",
	    (int) child_pid, ret);
	sleep(1);
	print_proc_name("FILHO DEPOIS", child_pid);
	waitpid(child_pid, NULL, 0);
	printf("\n");

	/* Teste 3: Validacao de erros (PID inexistente e nome vazio) */
	printf("3) Testes de validacao de erro:\n");
	errno = 0;
	ret = set_custom_proc_name(29999, "inexistente");
	printf("  PID inexistente (29999) -> retorno = %d, errno = %d (%s)\n",
	    ret, errno, strerror(errno));

	errno = 0;
	ret = set_custom_proc_name(target_pid, "");
	printf("  Nome vazio (\"\")         -> retorno = %d, errno = %d (%s)\n",
	    ret, errno, strerror(errno));

	printf("\nTodos os testes concluidos!\n");
	printf("Dica: tambem pode rodar './test_set_proc_name <pid> <nome>' para renomear qualquer processo.\n");
	return 0;
}
