#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================================
// Estrutura do Funcionário
// ================================
enum Status {INATIVO=0, ATIVO, PENDENTE};
enum Cargo (gerente, funcionario, estagiario);
typedef struct {
    int id;                     // ID único
    char nome[50];              // Nome do funcionário   
                            // 1 = ativo, 0 = removido
    enum Cargo cargo;
    enum status ativo;
        struct {
            int dia;
            int mes;
            int ano;
        } aniversario;
    int tipo_cargo; // 1 = gerente, 2 = funcionário, 3 = estagiário
} Funcionario;


int main() {
    FILE *arquivo = fopen("funcionarios.dat", "r+b");
    if (!arquivo) {
        arquivo = fopen("funcionarios.dat", "w+b");
        if (!arquivo) {
            printf("Erro ao abrir o arquivo.\n");
            return 1;
        }
    }

    menu(arquivo);
    fclose(arquivo);
    return 0;
}
