#ifndef FUNCIONARIOS_H
#define FUNCIONARIOS_H

// Status do funcionário 
enum Status { INATIVO = 0, ATIVO, PENDENTE };

// Papel / função geral 
enum FuncaoPapel {
    PAPEL_OUTRO = 0,
    PAPEL_DESENVOLVEDOR = 1,
    PAPEL_FAXINEIRO = 2,
    PAPEL_GERENTE = 3,
    PAPEL_SEGURANCA = 4
};

// Detalhes do desenvolvedor 
enum DevNivel { NIVEL_ESTAGIARIO = 0, NIVEL_JUNIOR, NIVEL_PLENO, NIVEL_SENIOR };
enum DevLinguagem { LING_C = 0, LING_PYTHON, LING_JAVA, LING_OUTRA };

typedef struct {
    enum DevNivel nivel;
    enum DevLinguagem linguagem;
} InfoDesenvolvedor;

// Detalhes do faxineiro 
enum FaxAndar { ANDAR_1 = 1, ANDAR_2 = 2, ANDAR_3 = 3 };
enum FaxCargo { FAX_COMUM = 0, FAX_CHEFE = 1 };

typedef struct {
    enum FaxAndar andar;
    enum FaxCargo cargo_fax;
} InfoFaxineiro;

// Detalhes do gerente 
enum GerenciaFuncao { GER_BANCARIO = 0, GER_JUDICIARIO, GER_ADMINISTRATIVO };
enum GerCargo { GERENTE_CHEFE = 0, GER_SUBGERENTE, GER_COMUM, GER_JOVEM_APRENDIZ };

typedef struct {
    enum GerenciaFuncao funcao;
    enum GerCargo cargo_ger;
} InfoGerenteDet;

// Detalhes da segurança 
enum SegLocal { LOC_ANDAR1 = 1, LOC_ANDAR2 = 2, LOC_ANDAR3 = 3, LOC_ENTRADA = 4 };
enum SegCargo { SEG_SUPERVISOR = 0, SEG_COMUM = 1 };

typedef struct {
    enum SegLocal local;
    enum SegCargo cargo_seg;
} InfoSeguranca;

// Union com as informações específicas do cargo 
typedef union {
    InfoDesenvolvedor dev;
    InfoFaxineiro fax;
    InfoGerenteDet ger;
    InfoSeguranca seg;
} CargoInfo;

// Estrutura principal do funcionário 
typedef struct {
    int id;
    char nome[50];
    enum Status ativo;
    enum FuncaoPapel papel;
    CargoInfo papel_info;
    unsigned char aniversario[31][12]; // marcação dia/mes 
    unsigned char tempo_empresa; // anos (0..30) 
} Funcionario;

#endif 
