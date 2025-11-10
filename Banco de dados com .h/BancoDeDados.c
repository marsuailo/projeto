#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funcionarios.h" 

const char *NOME_ARQUIVO = "funcionarios.txt";
const char *NOME_LISTAGEM = "listagem.funcionarios.txt";

   //utilitários

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
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

//ID
const char* nome_papel(enum FuncaoPapel p) {
    switch (p) {
        case PAPEL_DESENVOLVEDOR: return "Desenvolvedor";
        case PAPEL_FAXINEIRO: return "Faxineiro";
        case PAPEL_GERENTE: return "Gerente (detalhado)";
        case PAPEL_SEGURANCA: return "Seguranca";
        default: return "Nenhum";
    }
}

int proximo_id_txt(const char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return 1;
    int id = 0;
    char buffer[512];
    while (fgets(buffer, sizeof(buffer), f)) {
        int len = (int)strlen(buffer);
        bool only_space = true;
        for (int i = 0; i < len; i++)
            if (buffer[i] != '\n' && buffer[i] != '\r') { only_space = false; break; }
        if (!only_space) id++;
    }
    fclose(f);
    return id + 1;
}

int gravar_funcionario_txt(const char *nome_arquivo, const Funcionario *f) {
    FILE *out = fopen(nome_arquivo, "a");
    if (!out) return 0;
    int d = 0, m = 0;
    verifica_aniversario(f, &d, &m);
    int f1 = 0, f2 = 0;
    switch (f->papel) {
        case PAPEL_DESENVOLVEDOR: f1=(int)f->papel_info.dev.nivel; f2=(int)f->papel_info.dev.linguagem; break;
        case PAPEL_FAXINEIRO: f1=(int)f->papel_info.fax.andar; f2=(int)f->papel_info.fax.cargo_fax; break;
        case PAPEL_GERENTE: f1=(int)f->papel_info.ger.funcao; f2=(int)f->papel_info.ger.cargo_ger; break;
        case PAPEL_SEGURANCA: f1=(int)f->papel_info.seg.local; f2=(int)f->papel_info.seg.cargo_seg; break;
        default: f1=0; f2=0;
    }
    fprintf(out, "%d|%d|%d|%u|%d|%d|%d|%d|%s\n",
            f->id, (int)f->ativo, (int)f->papel, (unsigned int)f->tempo_empresa, d, m, f1, f2, f->nome);
    fclose(out);
    return 1;
}

Funcionario *carregar_todos_txt(const char *nome_arquivo, int *out_count) {
    *out_count = 0;
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return NULL;
    int cap = 16;
    Funcionario *vet = malloc(sizeof(Funcionario) * cap);
    if (!vet) { fclose(f); return NULL; }
    char linha[1024];
    while (fgets(linha, sizeof(linha), f)) {
        int llen = (int)strlen(linha);
        bool allspace = true;
        for (int i=0;i<llen;i++) if (linha[i] != '\n' && linha[i] != '\r') { allspace = false; break; }
        if (allspace) continue;

        Funcionario ff; memset(&ff,0,sizeof(ff)); limpar_aniversario(&ff);
        int id, ativo, papel, tempo, dia, mes, f1, f2;
        char nomebuf[256] = {0};
        int parsed = sscanf(linha, "%d|%d|%d|%d|%d|%d|%d|%d|%255[^\n\r]",
                            &id, &ativo, &papel, &tempo, &dia, &mes, &f1, &f2, nomebuf);
        if (parsed < 8) continue;
        ff.id = id; ff.ativo = (enum Status)ativo; ff.papel = (enum FuncaoPapel)papel;
        ff.tempo_empresa = (unsigned char)tempo;
        if (dia>=1 && mes>=1) define_aniversario(&ff, dia, mes);
        strncpy(ff.nome, nomebuf, sizeof(ff.nome)-1); ff.nome[sizeof(ff.nome)-1] = '\0';
        switch (ff.papel) {
            case PAPEL_DESENVOLVEDOR: ff.papel_info.dev.nivel = (enum DevNivel)f1; ff.papel_info.dev.linguagem = (enum DevLinguagem)f2; break;
            case PAPEL_FAXINEIRO: ff.papel_info.fax.andar = (enum FaxAndar)f1; ff.papel_info.fax.cargo_fax = (enum FaxCargo)f2; break;
            case PAPEL_GERENTE: ff.papel_info.ger.funcao = (enum GerenciaFuncao)f1; ff.papel_info.ger.cargo_ger = (enum GerCargo)f2; break;
            case PAPEL_SEGURANCA: ff.papel_info.seg.local = (enum SegLocal)f1; ff.papel_info.seg.cargo_seg = (enum SegCargo)f2; break;
            default: break;
        }
        if (*out_count >= cap) {
            cap *= 2;
            Funcionario *tmp = realloc(vet, sizeof(Funcionario)*cap);
            if (!tmp) { free(vet); fclose(f); *out_count=0; return NULL; }
            vet = tmp;
        }
        vet[(*out_count)++] = ff;
    }
    fclose(f);
    return vet;
}

int salvar_todos_txt(const char *nome_arquivo, Funcionario *vet, int count) {
    FILE *f = fopen(nome_arquivo, "w");
    if (!f) return 0;
    for (int i=0;i<count;i++) {
        Funcionario *p = &vet[i];
        int d=0,m=0; verifica_aniversario(p,&d,&m);
        int f1=0,f2=0;
        switch (p->papel) {
            case PAPEL_DESENVOLVEDOR: f1=(int)p->papel_info.dev.nivel; f2=(int)p->papel_info.dev.linguagem; break;
            case PAPEL_FAXINEIRO: f1=(int)p->papel_info.fax.andar; f2=(int)p->papel_info.fax.cargo_fax; break;
            case PAPEL_GERENTE: f1=(int)p->papel_info.ger.funcao; f2=(int)p->papel_info.ger.cargo_ger; break;
            case PAPEL_SEGURANCA: f1=(int)p->papel_info.seg.local; f2=(int)p->papel_info.seg.cargo_seg; break;
            default: f1=0; f2=0;
        }
        fprintf(f, "%d|%d|%d|%u|%d|%d|%d|%d|%s\n",
                p->id, (int)p->ativo, (int)p->papel, (unsigned int)p->tempo_empresa, d,m,f1,f2,p->nome);
    }
    fclose(f);
    return 1;
}

//Leitura

void ler_info_desenvolvedor(InfoDesenvolvedor *d) {
    int tmp;
    printf("Nivel (0-Estagiario, 1-Junior, 2-Pleno, 3-Senior): ");
    if (scanf("%d", &tmp) != 1) { limpar_buffer(); tmp = 0; }
    d->nivel = (tmp >= 0 && tmp <= 3) ? (enum DevNivel)tmp : NIVEL_ESTAGIARIO;

    printf("Linguagem (0-C,1-Python,2-Java,3-Outra): ");
    if (scanf("%d", &tmp) != 1) { limpar_buffer(); tmp = 3; }
    d->linguagem = (tmp >= 0 && tmp <= 3) ? (enum DevLinguagem)tmp : LING_OUTRA;
}

void ler_info_faxineiro(InfoFaxineiro *f) {
    int tmp;
    printf("Andar (1,2,3): ");
    if (scanf("%d", &tmp) != 1) { limpar_buffer(); tmp = 1; }
    f->andar = (tmp >= 1 && tmp <=3) ? (enum FaxAndar)tmp : ANDAR_1;

    printf("Cargo (0-Comum,1-Chefe): ");
    if (scanf("%d", &tmp) != 1) { limpar_buffer(); tmp = 0; }
    f->cargo_fax = (tmp == 1) ? FAX_CHEFE : FAX_COMUM;
}

void ler_info_gerente(InfoGerenteDet *g) {
    int tmp;
    printf("Funcao do gerente (0-Bancario,1-Judiciario,2-Administrativo): ");
    if (scanf("%d", &tmp) != 1) { limpar_buffer(); tmp = 2; }
    g->funcao = (tmp >=0 && tmp <=2) ? (enum GerenciaFuncao)tmp : GER_ADMINISTRATIVO;

    printf("Cargo do gerente (0-Gerente,1-Subgerente,2-Comum,3-Jovem Aprendiz): ");
    if (scanf("%d", &tmp) != 1) { limpar_buffer(); tmp = 2; }
    g->cargo_ger = (tmp >=0 && tmp <=3) ? (enum GerCargo)tmp : GER_COMUM;
}

void ler_info_seguranca(InfoSeguranca *s) {
    int tmp;
    printf("Local de atuação (1-Andar1,2-Andar2,3-Andar3,4-Entrada): ");
    if (scanf("%d", &tmp) != 1) { limpar_buffer(); tmp = 4; }
    s->local = (tmp >=1 && tmp <=4) ? (enum SegLocal)tmp : LOC_ENTRADA;

    printf("Cargo (0-Supervisor,1-Comum): ");
    if (scanf("%d", &tmp) != 1) { limpar_buffer(); tmp = 1; }
    s->cargo_seg = (tmp == 0) ? SEG_SUPERVISOR : SEG_COMUM;
}

//Operações: criar/listar/buscar/editar/excluir 

void criar_funcionario_txt(void) {
    Funcionario f; memset(&f,0,sizeof(f)); limpar_aniversario(&f);
    f.id = proximo_id_txt(NOME_ARQUIVO);
    f.ativo = ATIVO; f.papel = PAPEL_NENHUM; f.tempo_empresa = 0;
    limpar_buffer();
    printf("\n--- Cadastro Novo Funcionario ---\n");
    printf("Nome: ");
    fgets(f.nome, sizeof(f.nome), stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';

    int papel_choice;
    printf("Papel/Funcao especifica (1-Desenvolvedor,2-Faxineiro,3-Gerente,4-Seguranca,0-Nenhum): ");
    if (scanf("%d", &papel_choice) != 1) { limpar_buffer(); papel_choice = 0; }
    switch (papel_choice) {
        case 1: f.papel = PAPEL_DESENVOLVEDOR; ler_info_desenvolvedor(&f.papel_info.dev); break;
        case 2: f.papel = PAPEL_FAXINEIRO; ler_info_faxineiro(&f.papel_info.fax); break;
        case 3: f.papel = PAPEL_GERENTE; ler_info_gerente(&f.papel_info.ger); break;
        case 4: f.papel = PAPEL_SEGURANCA; ler_info_seguranca(&f.papel_info.seg); break;
        default: f.papel = PAPEL_NENHUM; break;
    }

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

    if (!gravar_funcionario_txt(NOME_ARQUIVO, &f)) printf("Erro ao gravar no arquivo texto.\n");
    else printf("\nFuncionario cadastrado com sucesso! (ID: %d)\n", f.id);
}

void imprimir_info_papel_terminal(const Funcionario *f) {
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
                   x->andar, (x->cargo_fax == FAX_CHEFE) ? "Faxineiro-chefe" : "Comum");
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
                   local_str, (s->cargo_seg == SEG_SUPERVISOR) ? "Supervisor" : "Comum");
            break;
        }
        default:
            printf("Papel: N/A\n");
            break;
    }
}

//arquivo:

void imprimir_info_papel_arquivo(FILE *dest, const Funcionario *f) {
    switch (f->papel) {
        case PAPEL_DESENVOLVEDOR: {
            const InfoDesenvolvedor *d = &f->papel_info.dev;
            const char *nivel_str = (d->nivel == NIVEL_ESTAGIARIO) ? "Estagiario" :
                                    (d->nivel == NIVEL_JUNIOR) ? "Junior" :
                                    (d->nivel == NIVEL_PLENO) ? "Pleno" : "Senior";
            const char *ling_str = (d->linguagem == LING_C) ? "C" :
                                   (d->linguagem == LING_PYTHON) ? "Python" :
                                   (d->linguagem == LING_JAVA) ? "Java" : "Outra";
            fprintf(dest, "Papel: Desenvolvedor (%s, %s)\n", nivel_str, ling_str);
            break;
        }
        case PAPEL_FAXINEIRO: {
            const InfoFaxineiro *x = &f->papel_info.fax;
            fprintf(dest, "Papel: Faxineiro (Andar: %d, %s)\n",
                   x->andar, (x->cargo_fax == FAX_CHEFE) ? "Faxineiro-chefe" : "Comum");
            break;
        }
        case PAPEL_GERENTE: {
            const InfoGerenteDet *g = &f->papel_info.ger;
            const char *funcao_str = (g->funcao == GER_BANCARIO) ? "Bancario" :
                                     (g->funcao == GER_JUDICIARIO) ? "Judiciario" : "Administrativo";
            const char *cargo_str = (g->cargo_ger == GERENTE_PLENO) ? "Gerente" :
                                    (g->cargo_ger == GER_SUBGERENTE) ? "Subgerente" :
                                    (g->cargo_ger == GER_COMUM) ? "Comum" : "Jovem Aprendiz";
            fprintf(dest, "Papel: Gerente (Funcao: %s, Cargo: %s)\n", funcao_str, cargo_str);
            break;
        }
        case PAPEL_SEGURANCA: {
            const InfoSeguranca *s = &f->papel_info.seg;
            const char *local_str = (s->local == LOC_ANDAR1) ? "1 andar" :
                                    (s->local == LOC_ANDAR2) ? "2 andar" :
                                    (s->local == LOC_ANDAR3) ? "3 andar" : "Entrada";
            fprintf(dest, "Papel: Seguranca (Local: %s, %s)\n",
                   local_str, (s->cargo_seg == SEG_SUPERVISOR) ? "Supervisor" : "Comum");
            break;
        }
        default:
            fprintf(dest, "Papel: N/A\n");
            break;
    }
}

void listar_funcionarios_txt(void) {
    int n = 0;
    Funcionario *vet = carregar_todos_txt(NOME_ARQUIVO, &n);
    if (!vet) { printf("\nNenhum registro encontrado ou arquivo inexistente.\n"); return; }
    printf("\n--- Lista de Funcionarios Ativos ---\n");
    for (int i = 0; i < n; i++) {
        Funcionario *f = &vet[i];
        if (f->ativo == ATIVO) {
            int d = 0, m = 0;
            printf("\nID: %d\n", f->id);
            printf("Nome: %s\n", f->nome[0] ? f->nome : "(sem nome)");
            if (verifica_aniversario(f,&d,&m)) printf("Nascimento: %02d/%02d\n", d, m);
            else printf("Nascimento: N/A\n");
            imprimir_info_papel_terminal(f);
            printf("Tempo na empresa: %u anos\n", (unsigned int)f->tempo_empresa);
        }
    }
    free(vet);
}

void buscar_funcionario_txt(void) {
    int id;
    printf("Digite o ID do funcionario para buscar: ");
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }
    int n = 0;
    Funcionario *vet = carregar_todos_txt(NOME_ARQUIVO, &n);
    if (!vet) { printf("Arquivo vazio ou inexistente.\n"); return; }
    for (int i = 0; i < n; i++) {
        if (vet[i].id == id) {
            if (vet[i].ativo == ATIVO) {
                int d = 0, m = 0;
                printf("\n--- Funcionario Encontrado ---\n");
                printf("ID: %d\n", vet[i].id);
                printf("Nome: %s\n", vet[i].nome);
                if (verifica_aniversario(&vet[i], &d, &m)) printf("Nascimento: %02d/%02d\n", d, m);
                else printf("Nascimento: N/A\n");
                imprimir_info_papel_terminal(&vet[i]);
                printf("Tempo na empresa: %u anos\n", (unsigned int)vet[i].tempo_empresa);
            } else {
                printf("Este funcionario esta inativo.\n");
            }
            free(vet);
            return;
        }
    }
    free(vet);
    printf("Funcionario nao encontrado.\n");
}

void editar_funcionario_txt(void) {
    int id;
    printf("Digite o ID do funcionario para editar: ");
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }
    int n = 0;
    Funcionario *vet = carregar_todos_txt(NOME_ARQUIVO, &n);
    if (!vet) { printf("Arquivo vazio ou inexistente.\n"); return; }
    for (int i = 0; i < n; i++) {
        if (vet[i].id == id) {
            if (vet[i].ativo != ATIVO) { printf("Este funcionario está inativo e nao pode ser editado.\n"); free(vet); return; }
            limpar_buffer();
            char novo_nome[50];
            printf("Nome (atual: %s)\n", vet[i].nome);
            printf("Novo nome (pressione enter para manter): ");
            fgets(novo_nome, sizeof(novo_nome), stdin);
            if (novo_nome[0] != '\n') {
                novo_nome[strcspn(novo_nome, "\n")] = '\0';
                strncpy(vet[i].nome, novo_nome, sizeof(vet[i].nome)-1);
                vet[i].nome[sizeof(vet[i].nome)-1] = '\0';
            }
            int papel_choice;
            printf("Papel atual: %s\n", nome_papel(vet[i].papel));
            printf("Novo papel (1-Desenvolvedor,2-Faxineiro,3-Gerente,4-Seguranca,0-Manter atual): ");
            if (scanf("%d", &papel_choice) == 1) {
                if (papel_choice >= 1 && papel_choice <= 4) {
                    vet[i].papel = (enum FuncaoPapel)papel_choice;
                    switch (vet[i].papel) {
                        case PAPEL_DESENVOLVEDOR: ler_info_desenvolvedor(&vet[i].papel_info.dev); break;
                        case PAPEL_FAXINEIRO: ler_info_faxineiro(&vet[i].papel_info.fax); break;
                        case PAPEL_GERENTE: ler_info_gerente(&vet[i].papel_info.ger); break;
                        case PAPEL_SEGURANCA: ler_info_seguranca(&vet[i].papel_info.seg); break;
                        default: break;
                    }
                }
            } else { limpar_buffer(); }
            int anos = -1;
            printf("Tempo na empresa atual: %u anos\n", (unsigned int)vet[i].tempo_empresa);
            printf("Novo tempo na empresa (0-30, -1 para manter): ");
            if (scanf("%d", &anos) == 1) { if (anos >= 0 && anos <= 30) vet[i].tempo_empresa = (unsigned char)anos; } else { limpar_buffer(); }
            int dia = 0, mes = 0;
            int cur_d = 0, cur_m = 0;
            verifica_aniversario(&vet[i], &cur_d, &cur_m);
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
            if (!(dia == 0 && mes == 0)) {
                if (dia == 0) dia = cur_d ? cur_d : 1;
                if (mes == 0) mes = cur_m ? cur_m : 1;
                define_aniversario(&vet[i], dia, mes);
            }
            if (!salvar_todos_txt(NOME_ARQUIVO, vet, n)) printf("Erro ao salvar alteracoes no arquivo.\n");
            else printf("Funcionario atualizado com sucesso!\n");
            free(vet); return;
        }
    }
    free(vet);
    printf("Funcionario nao encontrado.\n");
}

void excluir_funcionario_txt(void) {
    int id;
    printf("Digite o ID do funcionario para remover: ");
    if (scanf("%d", &id) != 1) { limpar_buffer(); printf("ID invalido.\n"); return; }
    int n = 0;
    Funcionario *vet = carregar_todos_txt(NOME_ARQUIVO, &n);
    if (!vet) { printf("Arquivo vazio ou inexistente.\n"); return; }
    for (int i = 0; i < n; i++) {
        if (vet[i].id == id) {
            if (vet[i].ativo == ATIVO) {
                vet[i].ativo = INATIVO;
                if (!salvar_todos_txt(NOME_ARQUIVO, vet, n)) printf("Erro ao marcar inativo no arquivo.\n");
                else printf("Funcionario marcado como inativo.\n");
            } else {
                printf("Esse funcionario ja esta inativo.\n");
            }
            free(vet); return;
        }
    }
    free(vet);
    printf("Funcionario nao encontrado.\n");
}

//exportar

int exportar_listagem(const char *nome_saida) {
    int n = 0;
    Funcionario *vet = carregar_todos_txt(NOME_ARQUIVO, &n);
    if (!vet) return 0;
    FILE *out = fopen(nome_saida, "w");
    if (!out) { free(vet); return 0; }
    fprintf(out, "--- Lista completa de funcionarios (apenas os ativos exibidos) ---\n");
    for (int i = 0; i < n; i++) {
        Funcionario *f = &vet[i];
        if (f->ativo != ATIVO) continue;
        int d = 0, m = 0; verifica_aniversario(f,&d,&m);
        fprintf(out, "\nID: %d\n", f->id);
        fprintf(out, "Nome: %s\n", f->nome[0] ? f->nome : "(sem nome)");
        if (d && m) fprintf(out, "Nascimento: %02d/%02d\n", d, m);
        else fprintf(out, "Nascimento: N/A\n");
        imprimir_info_papel_arquivo(out, f);
        fprintf(out, "Tempo na empresa: %u anos\n", (unsigned int)f->tempo_empresa);
    }
    fclose(out);
    free(vet);
    return 1;
}

void menu(void) {
    int opcao = -1;
    while (opcao != 0) {
        printf("\n=== SISTEMA DE FUNCIONARIOS ===\n");
        printf("1. Cadastrar funcionario\n");
        printf("2. Buscar funcionario\n");
        printf("3. Editar funcionario\n");
        printf("4. Excluir funcionario\n");
        printf("5. Listar funcionarios\n");
        printf("6. Exportar listagem para arquivo .txt\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) { limpar_buffer(); opcao = -1; }
        switch (opcao) {
            case 1: criar_funcionario_txt(); break;
            case 2: buscar_funcionario_txt(); break;
            case 3: editar_funcionario_txt(); break;
            case 4: excluir_funcionario_txt(); break;
            case 5: listar_funcionarios_txt(); break;
            case 6:
                if (exportar_listagem(NOME_LISTAGEM)) printf("Exportado para '%s' com sucesso.\n", NOME_LISTAGEM);
                else printf("Erro ao exportar para '%s'.\n", NOME_LISTAGEM);
                break;
            case 0: printf("código encerrado com sucesso!\n"); break;
            default: printf("Opcao invalida!\n");
        }
    }
}

int main(void) {
    FILE *f = fopen(NOME_ARQUIVO, "a");
    if (!f) { printf("Erro ao acessar/criar o arquivo '%s'.\n", NOME_ARQUIVO); return 1; }
    fclose(f);
    menu();
    return 0;
}
