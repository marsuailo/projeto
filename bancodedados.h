

/*
 *  (Versão Comentada)
 * Este arquivo define tipos de dados para uma aplicação que gerencia funcionários
 * que podem exercer diferentes papéis. Utiliza o conceito de "união discriminada",
 * onde uma enumeração determina qual estrutura dentro de uma união está ativa.
 */


/* ---------------------------------------------------------------------------
 * STATUS DO FUNCIONÁRIO
 * Define se o funcionário está ativo, inativo ou pendente.
 * --------------------------------------------------------------------------- */
typedef enum {
    STATUS_INATIVO = 0,
    STATUS_ATIVO,
    STATUS_PENDENTE
} Status;

/* ---------------------------------------------------------------------------
 * PAPEL DO FUNCIONÁRIO (DISCRIMINADOR DA UNIÃO)
 * O valor desta enumeração define qual campo da união 'PapelInfo' é válido.
 * --------------------------------------------------------------------------- */
typedef enum {
    PAPEL_NENHUM = 0,
    PAPEL_DESENVOLVEDOR = 1,
    PAPEL_FAXINEIRO   = 2,
    PAPEL_GERENTE     = 3,
    PAPEL_SEGURANCA   = 4
} FuncaoPapel;

/* ---------------------------------------------------------------------------
 * INFORMAÇÕES ESPECÍFICAS PARA DESENVOLVEDORES
 * --------------------------------------------------------------------------- */
typedef enum { NIVEL_ESTAGIARIO = 0, NIVEL_JUNIOR, NIVEL_PLENO, NIVEL_SENIOR } DevNivel;
typedef enum { LING_C = 0, LING_PYTHON, LING_JAVA, LING_OUTRA } DevLinguagem;

/* Armazena dados exclusivos de um desenvolvedor */
typedef struct {
    DevNivel nivel;          /* Nível do desenvolvedor (estagiário → sênior) */
    DevLinguagem linguagem;  /* Linguagem de programação predominante */
} InfoDesenvolvedor;

/* ---------------------------------------------------------------------------
 * INFORMAÇÕES ESPECÍFICAS PARA FAXINEIROS
 * --------------------------------------------------------------------------- */
typedef enum { ANDAR_1 = 1, ANDAR_2 = 2, ANDAR_3 = 3 } FaxAndar;
typedef enum { FAX_COMUM = 0, FAX_CHEFE = 1 } FaxCargo;

/* Armazena dados exclusivos de um faxineiro */
typedef struct {
    FaxAndar andar;   /* Andar responsável */
    FaxCargo cargo;   /* Cargo dentro da equipe */
} InfoFaxineiro;

/* ---------------------------------------------------------------------------
 * INFORMAÇÕES ESPECÍFICAS PARA GERENTES
 * --------------------------------------------------------------------------- */
typedef enum { GER_BANCARIO = 0, GER_JUDICIARIO, GER_ADMINISTRATIVO } GerenciaFuncao;
typedef enum { GERENTE_PLENO = 0, GER_SUBGERENTE, GER_COMUM, GER_JOVEM_APRENDIZ } GerCargo;

/* Armazena dados exclusivos de um gerente */
typedef struct {
    GerenciaFuncao funcao; /* Área de atuação */
    GerCargo cargo;        /* Cargo hierárquico */
} InfoGerente;

/* ---------------------------------------------------------------------------
 * INFORMAÇÕES ESPECÍFICAS PARA SEGURANÇAS
 * --------------------------------------------------------------------------- */
typedef enum { LOC_ANDAR1 = 1,_
