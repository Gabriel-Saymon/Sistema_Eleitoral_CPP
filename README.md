# Vereadores — Apuração Proporcional (C++)

Projeto acadêmico em **C++17** que replica a lógica de um trabalho prévio em Java para apurar a eleição de **vereadores (CD_CARGO = 13)** a partir dos arquivos oficiais do TSE (CSV).  
O programa lê os **candidatos** e a **votação** de um município, aplica as regras de contagem e gera exatamente os relatórios exigidos no enunciado — no **mesmo formato** de saída utilizado pelo script de correção do professor.

## 📦 Sumário
- [Objetivo](#-objetivo)
- [Relatórios gerados](#-relatórios-gerados)
- [Entrada: CLI e CSV](#-entrada-cli-e-csv)
- [Como compilar](#-como-compilar)
- [Como executar](#-como-executar)
- [Formato da saída (strict)](#-formato-da-saída-strict)
- [Arquitetura & Modelagem](#-arquitetura--modelagem)
- [Regras e detalhes importantes](#-regras-e-detalhes-importantes)
- [Estrutura do repositório](#-estrutura-do-repositório)
- [Testes e validação](#-testes-e-validação)
- [Solução de problemas (FAQ)](#-solução-de-problemas-faq)
- [Licença](#-licença)

---

## 🎯 Objetivo
- Implementar em **C++** a mesma apuração realizada no trabalho em **Java**, mudando apenas a linguagem e mantendo:
  - as **mesmas regras** de negócio,
  - os **mesmos arquivos de entrada**,
  - e o **mesmo formato de saída** (incluindo separadores de milhar, vírgulas decimais, acentos, hífens e espaçamentos).

---

## 🧾 Relatórios gerados
Na ordem:

1. **Número de vagas** (quantidade de eleitos no CSV de candidatos para o município e cargo 13).
2. **Vereadores eleitos** (ordem por votos nominais ↓; empate: mais velho ↑; com `*` para filiados em federação).
3. **Candidatos mais votados (Top N)** respeitando o número de vagas (mesmo critério de ordenação).
4. **Teriam sido eleitos na majoritária e não foram** (Top N que não estão entre os eleitos).
5. **Eleitos beneficiados pelo proporcional** (eleitos fora do Top N), incluindo **posição** no ranking geral.
6. **Votação por partido** (total = nominais + legenda; ordenação por total ↓; empate: menor número do partido).
7. **Primeiro e último de cada partido**  
   - Entra no relatório apenas partido com **votos nominais > 0**;  
   - **Primeiro**: mais votado entre candidatos com **> 0 voto** (empate: mais velho);  
   - **Último**: **menos votado** entre **todos os candidatos válidos** do partido (inclui 0); empate: **menor número do candidato** (fallback: mais velho).  
8. **Faixa etária dos eleitos** (`<30`, `30–39`, `40–49`, `50–59`, `≥60`) com percentuais **duas casas** usando vírgula.
9. **Gênero dos eleitos** (`2=Masculino`, `4=Feminino`) com percentuais.
10. **Totais de votos válidos**, **nominais** e **de legenda** (e percentuais).

---

## ⛽ Entrada: CLI e CSV

### Linha de comando
```
./vereadores <COD_MUNICIPIO> <candidatos.csv> <votos.csv> <DATA_ELEICAO dd/mm/aaaa>
```

**Exemplo:**
```bash
./vereadores 1200401 candidatos.csv votos.csv 06/10/2024
```

### CSVs (padrões esperados)
- Separador `;`, **todas as células entre aspas**, codificação **ISO-8859-1** (convertida para UTF-8 na leitura).
- **Candidatos** (principais colunas por índice):  
  - `11=SG_UE`, `13=CD_CARGO`, `16=NR_CANDIDATO`, `17=NM_URNA_CANDIDATO`, `18=NM_CANDIDATO`,  
    `25=NR_PARTIDO`, `26=SG_PARTIDO`, `28=NR_FEDERACAO`, `36=DT_NASCIMENTO`,  
    `38=CD_GENERO`, `48=CD_SIT_TOT_TURNO` (**-1 inválido**; **2/3 eleito**).
- **Votação**: filtrar por `CD_MUNICIPIO` e `CD_CARGO=13`.  
  - `NR_VOTAVEL` com **5 dígitos** → **voto nominal** para o candidato;  
  - `NR_VOTAVEL` com **2 dígitos** → **voto de legenda** para o partido;  
  - Códigos **95–98** → brancos/nulos/anulados (**ignorar**).

---

## 🛠️ Como compilar

Requisitos:
- **GCC/Clang** com C++17
- **make**

```bash
make clean && make
```

> O executável gerado deve se chamar **`vereadores`** (ajustado no `Makefile`).  
> Flags típicas: `-std=c++17 -O2 -Wall -Wextra`.

### Checagem de memória (opcional)
```bash
valgrind --leak-check=full ./vereadores 1200401 candidatos.csv votos.csv 06/10/2024
```

---

## ▶️ Como executar

```bash
./vereadores <COD_MUNICIPIO> <candidatos.csv> <votos.csv> <DATA_ELEICAO>
```

**Saída no STDOUT**, já formatada exatamente como o gabarito (acentos, vírgulas como separador decimal, pontos como milhar, pluralização de “voto/votos”).

---

## 🧾 Formato da saída (strict)

A correção automática compara **byte a byte**. Portanto:
- Inteiros com **ponto de milhar** (`7.256`).
- Percentuais com **vírgula** e **duas casas** (`3,35%`).
- Espaços, vírgulas, hífens, acentos e quebras de linha **exatamente** como nos exemplos do enunciado.
- Prefixar `*` no nome de candidatos de **federação**.

---

## 🧩 Arquitetura & Modelagem

- **`Candidato`**  
  Dados cadastrais (nº, nomes, partido, federação, nascimento, gênero, situação) e votos nominais acumulados.  
  Métodos de utilidade para idade na data da eleição e flags (`isEleito()`).

- **`Partido`**  
  Número e sigla; lista de candidatos; votos de **legenda**; somatórios de **nominais** e **total**.

- **`Eleicao`**  
  Código do município, data da eleição; mapas/coleções de candidatos/partidos; **nº de vagas** (conta `isEleito()`).

- **`Leitor`**  
  - Passagem 1: varre candidatos e cadastra **todos os partidos** encontrados.  
  - Passagem 2: cria **apenas candidatos válidos** (`CD_SIT_TOT_TURNO != -1`) do município/cargo.  
  - Rotina de votos: soma **nominais** (por candidato) e **legenda** (por partido); ignora **95–98**.

- **`Relatorio`**  
  Constrói visões ordenadas e **imprime** os 10 relatórios.  
  Usa um `numpunct` customizado para **pt-BR** (milhar com ponto, decimal com vírgula).

---

## 🧠 Regras e detalhes importantes

- **Idade**: calculada na **data da eleição** (considera mês/dia).
- **Eleitos**: `CD_SIT_TOT_TURNO ∈ {2, 3}`.
- **Inválidos**: **ignorar** (`-1`) — não entram no conjunto do partido.
- **Votos válidos**: apenas **nominais** (candidatos válidos) + **legenda** (partidos) do município/cargo; **95–98** ficam de fora.
- **Ordenações**:
  - Candidatos (listas 2–5): **votos ↓**; empate → **mais velho**.  
  - Partidos (lista 6): **total ↓**; empate → **menor NR_PARTIDO**.  
  - Lista 7 (“Primeiro/Último por partido”):  
    - Ordenar partidos por **voto do mais votado** do partido ↓; empate → **menor NR_PARTIDO**.  
    - **Primeiro**: entre `votos > 0` (↓; empate: mais velho).  
    - **Último**: **entre todos os válidos** (↑; empate: **menor `NR_CANDIDATO`**; fallback: mais velho).

---

## 🗂️ Estrutura do repositório

```
.
├── Candidato.h / Candidato.cpp
├── Partido.h   / Partido.cpp
├── Eleicao.h   / Eleicao.cpp
├── Leitor.h    / Leitor.cpp
├── Relatorio.h / Relatorio.cpp
├── Utils.h
├── main.cpp
├── Makefile
└── README.md   ← (este arquivo)
```

---

## ✅ Testes e validação

- O projeto foi validado com o **script de correção** do professor, cobrindo múltiplos municípios (casos AC/ES/PE).  
- Para testar localmente:
  ```bash
  make clean && make
  ./vereadores <COD> <candidatos.csv> <votos.csv> <DATA>
  ```
- Compare a saída com o gabarito fornecido para cada caso.  
- Dicas para reproduzir 100%:  
  - Use os **mesmos CSVs** do professor (sem editar).  
  - Garanta que o arquivo está em **ISO-8859-1** (a leitura converte para UTF-8).  
  - Não altere espaçamentos/acentos da impressão.

---

## 🆘 Solução de problemas (FAQ)

**A saída difere apenas no “Primeiro e último por partido”**  
→ Verifique se candidatos **inválidos** (`CD_SIT_TOT_TURNO == -1`) foram **ignorados** na leitura.  
→ Empates do “último” (mesma menor votação) são resolvidos por **menor `NR_CANDIDATO`** (fallback: mais velho).

**Acentos “quebrados”**  
→ Confira se os CSVs estão em **ISO-8859-1**. A leitura faz `ISO-8859-1 → UTF-8`.  
→ Evite reabrir e salvar os CSVs no editor (pode mudar a codificação).

**Percentuais e milhar não batem**  
→ O projeto usa `numpunct` customizado (`pt-BR`): **vírgula** para decimais, **ponto** para milhar.  
→ Não troque `std::cout.imbue(...)`/facet caso existente no código.

**Votos 95–98 aparecendo**  
→ Esses códigos representam **brancos/nulos/anulados** — **ignorar** na leitura dos votos.

---

## 📄 Licença
Defina a licença do repositório (ex.: MIT).  
_Sugestão:_ inclua um `LICENSE` na raiz.
