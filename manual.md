# **Manual do Usuário — *RH EMPRESA***

<img width="1024" height="1024" alt="image" src="https://github.com/user-attachments/assets/14320e9f-439f-4e9a-abcd-f05e20e70657" />

---

 **Visão Geral da Aplicação**

**RH EMPRESA** é uma aplicação para gerenciamento de funcionários com diferentes papéis estruturados por meio de "uniões discriminadas". Isso significa que cada funcionário possui um papel específico, e cada papel ativa um conjunto distinto de informações.

Este manual descreve todas as funções, operações e comportamentos da aplicação.

---

**Estrutura dos Funcionários**

Cada funcionário possui:

* **Status** (Ativo, Inativo ou Pendente)
* **Papel** (Desenvolvedor, Faxineiro, Gerente ou Segurança)
* **Informações específicas**, dependendo do papel

A aplicação utiliza um discriminador (`FuncaoPapel`) que define qual estrutura interna da união está ativa.

---

 **Papéis e Atributos**

### Desenvolvedor

Informações específicas:

* **Nível:** Estagiário, Júnior, Pleno, Sênior
* **Linguagem predominante:** C, Python, Java ou Outra

<img width="886" height="500" alt="image" src="https://github.com/user-attachments/assets/ffbb71e1-c82c-43c8-87ba-e82c80ebb954" />


---

### Faxineiro

Informações específicas:

* **Andar responsável:** 1, 2 ou 3
* **Cargo:** Comum ou Chefe

<img width="650" height="350" alt="image" src="https://github.com/user-attachments/assets/615ed46d-189f-49b5-a499-3309e39ce983" />

---

###Gerente

Informações específicas:

* **Função:** Bancário, Judiciário, Administrativo
* **Cargo:** Pleno, Subgerente, Comum ou Jovem Aprendiz

<img width="727" height="175" alt="image" src="https://github.com/user-attachments/assets/323019ba-3416-488f-abe6-5bc99be208d3" />


---

### Segurança

Informações específicas:

* **Local de atuação:** Andar 1, 2, 3 
* **Cargo:** Supervisor, comum
<img width="712" height="173" alt="image" src="https://github.com/user-attachments/assets/b1a075d1-836a-418e-8723-1e99be3e6890" />


---

## **Operações da Aplicação**

### **1. Cadastrar Funcionário**

Cria um novo registro com:

* Nome
* Status
* Papel
* Dados específicos do papel

**Efeito:** adiciona o funcionário ao sistema.

*Exemplo:* cadastrar um desenvolvedor Pleno que programa em C.

<img width="746" height="757" alt="image" src="https://github.com/user-attachments/assets/184b4c46-7de5-44c8-9995-845d460fe5cd" />


---

### **2. Alterar Status**

Permite definir o status do funcionário como:

* Ativo
* Inativo
* Pendente

**Efeito:** muda a disponibilidade operacional do funcionário.

<img width="680" height="362" alt="image" src="https://github.com/user-attachments/assets/de687845-be44-4e1f-9d83-5095fb9abb1b" />



---

### **3. Alterar Papel do Funcionário**

Ao mudar o papel:

* A antiga estrutura interna da união é descartada.
* A nova estrutura correspondente ao novo papel é criada.

**Efeito:** o funcionário assume outro conjunto de atributos.

*Atenção:* Dados anteriores do papel antigo são perdidos.

<img width="1112" height="603" alt="image" src="https://github.com/user-attachments/assets/fbb5a82c-38f5-4b86-a720-a75b8d61ebf1" />



---

### **4. Remover Funcionário**

Remove completamente o registro do funcionário.

**Efeito:** seu ID deixa de existir no sistema.

<img width="845" height="429" alt="image" src="https://github.com/user-attachments/assets/ac0c5e98-af37-42fe-9750-38ff15197a21" />


---

### **5. Visualizar Dados do Funcionário**

Mostra:

* Status
* Papel
* Informações específicas

<img width="790" height="542" alt="image" src="https://github.com/user-attachments/assets/8ede4893-a085-41d0-bb65-f61a6cd220be" />



---

## **Notas Importantes**

* A aplicação utiliza *enumerações* para garantir integridade de dados.
* Apenas a estrutura correspondente ao papel ativo pode ser acessada.
* Mudanças de papel substituem a estrutura ativa da união.

---

## **Contato e Suporte**

Para dúvidas ou suporte técnico, entre em contato com o administrador da aplicação.

---

### *Fim do manual*
