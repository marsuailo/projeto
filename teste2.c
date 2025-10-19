#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// ================================
// Estruturas e enums (novos)
// ================================

// Status e cargo geral (mantidos)
enum Status { INATIVO = 0, ATIVO, PENDENTE };
enum Cargo { GERENTE = 1, FUNCIONARIO, ESTAGIARIO };

// Função (papel) específico do funcionário — discriminador da union
enum FuncaoPapel {
    PAPEL_NENHUM = 0,
    PAPEL_DESENVOLVEDOR = 1,
    PAPEL_FAXINEIRO = 2,
    PAPEL_GERENTE = 3,
    PAPEL_SEGURANCA = 4
};

// --- Desenvolvedor ---
enum DevNivel { NIVEL_ESTAGIARIO = 0, NIVEL_JUNIOR, NIVEL_PLENO, NIVEL_SENIOR };
enum DevLinguagem { LING_C = 0, LING_PYTHON, LING_JAVA, LING_OUTRA };

typedef struct {
    enum DevNivel nivel;
    enum DevLinguagem linguagem;
} InfoDesenvolvedor;

// --- Faxineiro ---
enum FaxAndar { ANDAR_1 = 1, ANDAR_2 = 2, ANDAR_3 = 3 };
enum FaxCargo { FAX_COMUM = 0, FAX_CHEFE = 1 };

typedef struct {
    enum FaxAndar andar;
    enum FaxCargo cargo_fax;
} InfoFaxineiro;

// --- Gerente (detalhe adicional do cargo) ---
enum GerenciaFuncao { GER_BANCARIO = 0, GER_JUDICIARIO, GER_ADMINISTRATIVO };
enum GerCargo { GERENTE_PLENO = 0, GER_SUBGERENTE, GER_COMUM, GER_JOVEM_APRENDIZ };

typedef struct {
    enum GerenciaFuncao funcao;
    enum GerCargo cargo_ger;
} InfoGerenteDet;

// --- Segurança ---
enum SegLocal { LOC_ANDAR1 = 1, LOC_ANDAR2 = 2, LOC_ANDAR3 = 3, LOC_ENTRADA = 4 };
enum SegCargo { SEG_SUPERVISOR = 0, SEG_COMUM = 1 };

typedef struct {
    enum SegLocal local;
    enum SegCargo cargo_seg;
} InfoSeguranca;

// Union que guarda as informações específicas do papel
typedef union {
    InfoDesenvolvedor dev;
    InfoFaxineiro fax;
    InfoGerenteDet ger;
    InfoSeguranca seg;
} CargoInfo;

// ================================
// Estrutura do Funcionário (atualizada)
// ================================

typedef struct {
    int id;
    char nome[50];
    enum Cargo tipo_cargo;      // mantém compatibilidade com seu modelo original
    enum Status ativo;

    // campo novo: função/papel discriminador + union com dados específicos
    enum FuncaoPapel papel;
    CargoInfo papel_info;

    unsigned char aniversario[31][12]; // [0..30] => dias 1..31, [0..11] => meses 1..12

    // tempo na empresa (anos), 0..30
    unsigned char tempo_empresa;
} Funcionario;

// ================================
// Funções auxiliares
// ================================

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int proximo_id(FILE *arquivo) {
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    return (int)(tamanho / sizeof(Funcionario)) + 1;
}

void limpar_aniversario(Funcionario *f) {
    memset(f->aniversario, 0, sizeof(f->aniversario));
}

void define_aniversario(Funcionario *f, int dia, int mes) {
    limpar_aniversario(f);
    if (dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12) {
        f->aniversario[dia - 1][mes - 1] = 1;
    }
}

int verifica_aniversario(const Funcionario *f, int *dia, int *mes) {
    for (int d = 0; d < 31; d++) {
        for (int m = 0; m < 12; m++) {
            if (f->aniversario[d][m]) {
                if (dia) *dia = d + 1;
                if (mes) *mes = m + 1;
                return 1;
            }
        }
    }
    return 0;
}

// Funções utilitárias para imprimir enums legíveis
const char* nome_cargo_geral(enum Cargo c) {
    switch (c) {
        case GERENTE: return "Gerente";
        case FUNCIONARIO: return "Funcionario";
        case ESTAGIARIO: return "Estagiario";
        default: return "Desconhecido";
    }
}

const char* nome_papel(enum FuncaoPapel p) {
    switch (p) {
        case PAPEL_DESENVOLVEDOR: return "Desenvolvedor";
        case PAPEL_FAXINEIRO: return "Faxineiro";
        case PAPEL_GERENTE: return "Gerente (detalhado)";
        case PAPEL_SEGURANCA: return "Seguranca";
        default: return "Nenhum";
    }
}

// ================================
// Entrada específica do papel (leitura)
// ================================

void ler_info_desenvolvedor(InfoDesenvolvedor *d) {
    int tmp;
    printf("Nivel (0-Estagiario, 1-Junior, 2-Pleno, 3-Senior): ");
    if (scanf("%d", &tmp) != 1) tmp = 0;
    d->nivel = (tmp >= 0 && tmp <= 3) ? (enum DevNivel)tmp : NIVEL_ESTAGIARIO;

    printf("Linguagem (0-C,1-Python,2-Java,3-Outra): ");
    if (scanf("%d", &tmp) != 1) tmp = 3;
    d->linguagem = (tmp >= 0 && tmp <= 3) ? (enum DevLinguagem)tmp : LING_OUTRA;
}

void ler_info_faxineiro(InfoFaxineiro *f) {
    int tmp;
    printf("Andar (1,2,3): ");
    if (scanf("%d", &tmp) != 1) tmp = 1;
    f->andar = (tmp >= 1 && tmp <=3) ? (enum FaxAndar)tmp : ANDAR_1;

    printf("Cargo (0-Comum,1-Chefe): ");
    if (scanf("%d", &tmp) != 1) tmp = 0;
    f->cargo_fax = (tmp == 1) ? FAX_CHEFE : FAX_COMUM;
}

void ler_info_gerente(InfoGerenteDet *g) {
    int tmp;
    printf("Funcao do gerente (0-Bancario,1-Judiciario,2-Administrativo): ");
    if (scanf("%d", &tmp) != 1) tmp = 0;
    g->funcao = (tmp >=0 && tmp <=2) ? (enum GerenciaFuncao)tmp : GER_ADMINISTRATIVO;

    printf("Cargo do gerente (0-Gerente,1-Subgerente,2-Comum,3-Jovem Aprendiz): ");
    if (scanf("%d", &tmp) != 1) tmp = 2;
    g->cargo_ger = (tmp >=0 && tmp <=3) ? (enum GerCargo)tmp : GER_COMUM;
}

void ler_info_seguranca(InfoSeguranca *s) {
    int tmp;
    printf("Local de atuação (1-Andar1,2-Andar2,3-Andar3,4-Entrada): ");
    if (scanf("%d", &tmp) != 1) tmp = 4;
    s->local = (tmp >=1 && tmp <=4) ? (enum SegLocal)tmp : LOC_ENTRADA;

    printf("Cargo (0-Supervisor,1-Comum): ");
    if (scanf("%d", &tmp) != 1) tmp = 1;
    s->cargo_seg = (tmp == 0) ? SEG_SUPERVISOR : SEG_COMUM;
}

// ================================
// Criar funcionário (atualizado)
// ================================

void criar_funcionario(FILE *arquivo) {
    Funcionario f;
    f.id = proximo_id(arquivo);
    f.ativo = ATIVO;
    f.papel = PAPEL_NENHUM;
    f.tempo_empresa = 0;
    limpar_aniversario(&f);

    printf("\n--- Cadastro Novo Funcionario ---\n");
    limpar_buffer();
    printf("Nome: ");
    fgets(f.nome, sizeof(f.nome), stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';

    int tmpCargo;
    printf("Cargo (1-Gerente, 2-Funcionario, 3-Estagiario): ");
    if (scanf("%d", &tmpCargo) != 1) tmpCargo = 2;
    f.tipo_cargo = (tmpCargo >= 1 && tmpCargo <= 3) ? (enum Cargo)tmpCargo : FUNCIONARIO;

    // --- Perguntar papel/especialização (novo)
    int papel_choice;
    printf("Papel/Funcao especifica (1-Desenvolvedor,2-Faxineiro,3-Gerente,4-Seguranca,0-Nenhum): ");
    if (scanf("%d", &papel_choice) != 1) papel_choice = 0;
    switch (papel_choice) {
        case 1:
            f.papel = PAPEL_DESENVOLVEDOR;
            ler_info_desenvolvedor(&f.papel_info.dev);
            break;
        case 2:
            f.papel = PAPEL_FAXINEIRO;
            ler_info_faxineiro(&f.papel_info.fax);
            break;
        case 3:
            f.papel = PAPEL_GERENTE;
            ler_info_gerente(&f.papel_info.ger);
            break;
        case 4:
            f.papel = PAPEL_SEGURANCA;
            ler_info_seguranca(&f.papel_info.seg);
            break;
        default:
            f.papel = PAPEL_NENHUM;
            break;
    }

    // --- Tempo na empresa (0..30)
    int anos = -1;
    do {
        printf("Tempo na empresa (anos, 0-30): ");
        if (scanf("%d", &anos) != 1) { limpar_buffer(); anos = -1; }
        if (anos < 0 || anos > 30) printf("Valor invalido. Insira 0..30.\n");
    } while (anos < 0 || anos > 30);
    f.tempo_empresa = (unsigned char)anos;

    int dia = 0, mes = 0;
    do {
        printf("Data de nascimento - Dia (1-31): ");
        if (scanf("%d", &dia) != 1) { limpar_buffer(); dia = 0; }
        if (dia < 1 || dia > 31) printf("Dia invalido. Tente novamente.\n");
    } while (dia < 1 || dia > 31);

    do {
        printf("Data de nascimento - Mes (1-12): ");
        if (scanf("%d", &mes) != 1) { limpar_buffer(); mes = 0; }
        if (mes < 1 || mes > 12) printf("Mes invalido. Tente novamente.\n");
    } while (mes < 1 || mes > 12);

    define_aniversario(&f, dia, mes);

    fseek(arquivo, (f.id - 1) * sizeof(Funcionario), SEEK_SET);
    fwrite(&f, sizeof(Funcionario), 1, arquivo);
    fflush(arquivo);

    printf("\nFuncionario cadastrado com sucesso! (ID: %d)\n", f.id);
}

// ================================
// Imprimir detalhes do papel
// ================================
void imprimir_info_papel(const Funcionario *f) {
    switch (f->papel) {
        case PAPEL_DESENVOLVEDOR: {
            const InfoDesenvolvedor *d = &f->papel_info.dev;
            const char *nivel_str = (d->nivel == NIVEL_ESTAGIARIO) ? "Estagiario" :
                                    (d->nivel == NIVEL_JUNIOR) ? "Junior" :
                                    (d->nivel == NIVEL_PLENO) ? "Pleno" : "Senior";
            const char *ling_str = (d->linguagem == LING_C) ? "C" :
                                   (d->linguagem == LING_PYTHON) ? "Python" :
                                   (d->linguagem == LING_JAVA) ? "Java" : "Outra";
            printf("Papel: Desenvolvedor (%s, %s)\n", nivel_str, ling_str);
            break;
        }
        case PAPEL_FAXINEIRO: {
            const InfoFaxineiro *x = &f->papel_info.fax;
            printf("Papel: Faxineiro (Andar: %d, %s)\n",
                   x->andar,
                   (x->cargo_fax == FAX_CHEFE) ? "Faxineiro-chefe" : "Comum");
            break;
        }
        case PAPEL_GERENTE: {
            const InfoGerenteDet *g = &f->papel_info.ger;
            const char *funcao_str = (g->funcao == GER_BANCARIO) ? "Bancario" :
                                     (g->funcao == GER_JUDICIARIO) ? "Judiciario" : "Administrativo";
            const char *cargo_str = (g->cargo_ger == GERENTE_PLENO) ? "Gerente" :
                                    (g->cargo_ger == GER_SUBGERENTE) ? "Subgerente" :
                                    (g->cargo_ger == GER_COMUM) ? "Comum" : "Jovem Aprendiz";
            printf("Papel: Gerente (Funcao: %s, Cargo: %s)\n", funcao_str, cargo_str);
            break;
        }
        case PAPEL_SEGURANCA: {
            const InfoSeguranca *s = &f->papel_info.seg;
            const char *local_str = (s->local == LOC_ANDAR1) ? "1 andar" :
                                    (s->local == LOC_ANDAR2) ? "2 andar" :
                                    (s->local == LOC_ANDAR3) ? "3 andar" : "Entrada";
            printf("Papel: Seguranca (Local: %s, %s)\n",
                   local_str,
                   (s->cargo_seg == SEG_SUPERVISOR) ? "Supervisor" : "Comum");
            break;
        }
        default:
            printf("Papel: N/A\n");
            break;
    }
}

// ================================
// MOSTRAR FUNCIONARIOS (atualizado)
// ================================

void listar_funcionarios(FILE *arquivo) {
    Funcionario f;
    rewind(arquivo);

    printf("\n--- Lista de Funcionarios Ativos ---\n");
    while (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            int d = 0, m = 0;
            printf("\nID: %d\n", f.id);
            printf("Nome: %s\n", f.nome);
            if (verifica_aniversario(&f, &d, &m)) {
                printf("Nascimento: %02d/%02d\n", d, m);
            } else {
                printf("Nascimento: N/A\n");
            }
            printf("Cargo geral: %s\n", nome_cargo_geral(f.tipo_cargo));
            imprimir_info_papel(&f);
            printf("Tempo na empresa: %u anos\n", (unsigned int)f.tempo_empresa);
        }
    }
}

// ================================
// BUSCAR FUNCIONARIO (atualizado para papel/tempo)
// ================================

void buscar_funcionario(FILE *arquivo) {
    int id;
    printf("Digite o ID do funcionario para buscar: ");
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            int d = 0, m = 0;
            printf("\n--- Funcionario Encontrado ---\n");
            printf("ID: %d\n", f.id);
            printf("Nome: %s\n", f.nome);
            if (verifica_aniversario(&f, &d, &m)) {
                printf("Nascimento: %02d/%02d\n", d, m);
            } else {
                printf("Nascimento: N/A\n");
            }
            printf("Cargo geral: %s\n", nome_cargo_geral(f.tipo_cargo));
            imprimir_info_papel(&f);
            printf("Tempo na empresa: %u anos\n", (unsigned int)f.tempo_empresa);
        } else {
            printf("Este funcionario esta inativo.\n");
        }
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

// ================================
// EDITAR (adaptado para papel e tempo)
// ================================

void editar_funcionario(FILE *arquivo) {
    int id;
    printf("Digite o ID do funcionario para editar: ");
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            limpar_buffer();
            char novo_nome[50];
            printf("Nome (atual: %s)\n", f.nome);
            printf("Novo nome (pressione enter para manter): ");
            fgets(novo_nome, sizeof(novo_nome), stdin);
            if (novo_nome[0] != '\n') {
                novo_nome[strcspn(novo_nome, "\n")] = '\0';
                strncpy(f.nome, novo_nome, sizeof(f.nome));
                f.nome[sizeof(f.nome)-1] = '\0';
            }

            int tmpCargo;
            printf("Novo cargo (1-Gerente, 2-Funcionario, 3-Estagiario) (atual: %d): ", f.tipo_cargo);
            if (scanf("%d", &tmpCargo) == 1) {
                if (tmpCargo >= 1 && tmpCargo <= 3) f.tipo_cargo = (enum Cargo)tmpCargo;
            } else {
                limpar_buffer();
            }

            // editar papel específico
            int papel_choice;
            printf("Papel atual: %s\n", nome_papel(f.papel));
            printf("Novo papel (1-Desenvolvedor,2-Faxineiro,3-Gerente,4-Seguranca,0-Manter atual): ");
            if (scanf("%d", &papel_choice) == 1) {
                if (papel_choice >= 1 && papel_choice <= 4) {
                    f.papel = (enum FuncaoPapel)papel_choice;
                    // ler dados do novo papel
                    switch (f.papel) {
                        case PAPEL_DESENVOLVEDOR: ler_info_desenvolvedor(&f.papel_info.dev); break;
                        case PAPEL_FAXINEIRO: ler_info_faxineiro(&f.papel_info.fax); break;
                        case PAPEL_GERENTE: ler_info_gerente(&f.papel_info.ger); break;
                        case PAPEL_SEGURANCA: ler_info_seguranca(&f.papel_info.seg); break;
                        default: break;
                    }
                }
            } else {
                limpar_buffer();
            }

            // editar tempo na empresa
            int anos = -1;
            printf("Tempo na empresa atual: %u anos\n", (unsigned int)f.tempo_empresa);
            printf("Novo tempo na empresa (0-30, -1 para manter): ");
            if (scanf("%d", &anos) == 1) {
                if (anos >= 0 && anos <= 30) f.tempo_empresa = (unsigned char)anos;
            } else {
                limpar_buffer();
            }

            int dia = 0, mes = 0;
            int cur_d = 0, cur_m = 0;
            verifica_aniversario(&f, &cur_d, &cur_m);

            printf("Nova data de nascimento (Digite 0 0 para manter atual) (atual: %02d/%02d)\n", cur_d, cur_m);
            do {
                printf("Dia (0 para manter, 1-31 para alterar): ");
                if (scanf("%d", &dia) != 1) { limpar_buffer(); dia = -1; }
                if (dia < 0 || (dia > 31)) printf("Dia invalido. Tente novamente.\n");
            } while (dia < 0 || dia > 31);

            do {
                printf("Mes (0 para manter, 1-12 para alterar): ");
                if (scanf("%d", &mes) != 1) { limpar_buffer(); mes = -1; }
                if (mes < 0 || (mes > 12)) printf("Mes invalido. Tente novamente.\n");
            } while (mes < 0 || mes > 12);

            if (dia == 0 && mes == 0) {
                // manter atual
            } else {
                if (dia == 0) dia = cur_d ? cur_d : 1;
                if (mes == 0) mes = cur_m ? cur_m : 1;
                define_aniversario(&f, dia, mes);
            }

            fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
            fwrite(&f, sizeof(Funcionario), 1, arquivo);
            fflush(arquivo);

            printf("Funcionario atualizado com sucesso!\n");
        } else {
            printf("Este funcionario está inativo e nao pode ser editado.\n");
        }
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

// ================================
// EXCLUIR
// ================================

void excluir_funcionario(FILE *arquivo) {
    int id;
    printf("Digite o ID do funcionario para remover: ");
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }

    Funcionario f;
    fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
    if (fread(&f, sizeof(Funcionario), 1, arquivo)) {
        if (f.ativo == ATIVO) {
            f.ativo = INATIVO;
            fseek(arquivo, (id - 1) * sizeof(Funcionario), SEEK_SET);
            fwrite(&f, sizeof(Funcionario), 1, arquivo);
            fflush(arquivo);
            printf("Funcionario marcado como inativo.\n");
        } else {
            printf("Esse funcionario ja esta inativo.\n");
        }
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

// ================================
// Menu Principal
// ================================

void menu(FILE *arquivo) {
    int opcao = -1;

    while (opcao != 0) {
        printf("\n=== SISTEMA DE FUNCIONARIOS ===\n");
        printf("1. Cadastrar funcionario\n");
        printf("2. Buscar funcionario\n");
        printf("3. Editar funcionario\n");
        printf("4. Excluir funcionario\n");
        printf("5. Listar funcionarios\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); opcao = -1; }

        switch (opcao) {
            case 1: criar_funcionario(arquivo); break;
            case 2: buscar_funcionario(arquivo); break;
            case 3: editar_funcionario(arquivo); break;
            case 4: excluir_funcionario(arquivo); break;
            case 5: listar_funcionarios(arquivo); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    }
}

// ================================
// Função Principal
// ================================

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
