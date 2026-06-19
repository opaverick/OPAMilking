# Publicacao do OPA Milking

Este guia cobre publicacao na Steam Workshop e no GitHub.

## Antes de publicar

Checklist:

- Build final testado no servidor local.
- Cliente entra sem erro de assinatura.
- `OPAMilking.pbo` atualizado.
- `.bisign` atualizado.
- `.bikey` separada para donos de servidor.
- `mod.cpp` preenchido.
- `README.md` revisado.
- Nenhum arquivo temporario dentro da pasta do mod.

Estrutura recomendada para publicacao:

```txt
@OPAMilking/
├── Addons/
│   ├── OPAMilking.pbo
│   └── OPAMilking.pbo.OPAMilking.bisign
├── Keys/
│   └── OPAMilking.bikey
├── mod.cpp
├── README.md
└── PUBLICACAO.md
```

Na Steam Workshop, a pasta publicada deve ser a pasta `@OPAMilking`.

## Publicar na Steam Workshop

### 1. Preparar a pasta final

Crie uma pasta final limpa, por exemplo:

```txt
C:\Users\User\Desktop\Workshop\@OPAMilking
```

Dentro dela:

```txt
@OPAMilking/
├── Addons/
│   ├── OPAMilking.pbo
│   └── OPAMilking.pbo.OPAMilking.bisign
├── Keys/
│   └── OPAMilking.bikey
├── mod.cpp
├── README.md
└── PUBLICACAO.md
```

### 2. Abrir o Publisher

Use o DayZ Tools:

```txt
Steam > Library > Tools > DayZ Tools > Publisher
```

Ou abra:

```txt
C:\Program Files (x86)\Steam\steamapps\common\DayZ Tools\Bin\Publisher\Publisher.exe
```

### 3. Criar novo item

No Publisher:

1. Selecione `New`.
2. Escolha a pasta `@OPAMilking`.
3. Defina o titulo:

```txt
OPA Milking
```

4. Defina visibilidade inicial como `Hidden` ou `Friends Only` para teste.
5. Adicione imagem de capa.
6. Publique.

### 4. Testar o item publicado

Depois de publicar:

1. Assine o mod na Workshop.
2. Espere o Steam baixar.
3. Carregue o mod pelo launcher.
4. Entre no servidor local usando a versao baixada da Workshop.

So coloque como `Public` depois desse teste.

### 5. Atualizar o mod na Workshop

Para atualizar:

1. Rebuild do PBO.
2. Assine novamente o PBO.
3. Substitua os arquivos na pasta `@OPAMilking`.
4. Abra o Publisher.
5. Selecione o item existente.
6. Publique update.
7. Escreva changelog curto.

Exemplo de changelog:

```txt
- Improved milk container compatibility.
- Added configurable milking amount.
- Added configurable milkings per cow per restart.
```

## Publicar no GitHub

### 1. Criar repositorio

No GitHub:

1. Clique em `New repository`.
2. Nome recomendado:

```txt
OPAMilking
```

3. Visibilidade:

- `Private`, se o codigo ainda nao deve ser publico;
- `Public`, se for open source.

4. Nao precisa criar README pelo GitHub se ja existe localmente.

### 2. Arquivos recomendados no GitHub

Para repositorio de codigo-fonte:

```txt
OPAMilking/
├── config.cpp
├── mod.cpp
├── README.md
├── PUBLICACAO.md
├── sounds/
├── scripts/
└── .gitignore
```

Evite subir arquivos gerados:

```txt
*.pbo
*.bisign
*.biprivatekey
*.log
*.rpt
*.tmp
```

A chave privada `.biprivatekey` nunca deve ir para o GitHub.

### 3. Criar .gitignore

Use:

```gitignore
*.pbo
*.bisign
*.biprivatekey
*.log
*.rpt
*.mdmp
*.tmp
*.bak
Build/
Profiles/
```

### 4. Primeiro commit

Dentro da pasta do projeto:

```powershell
cd "C:\Users\User\Desktop\Opaverick Mod\OPAMilking"
git init
git add .
git commit -m "Initial OPA Milking release"
git branch -M main
git remote add origin https://github.com/SEU_USUARIO/OPAMilking.git
git push -u origin main
```

### 5. Releases no GitHub

Para cada versao:

1. Crie uma tag:

```powershell
git tag v1.0.0
git push origin v1.0.0
```

2. No GitHub, va em `Releases`.
3. Crie uma release com changelog.
4. Anexe, se quiser:

```txt
@OPAMilking.zip
```

Nao anexe a `.biprivatekey`.

## Versionamento recomendado

Use semver simples:

```txt
1.0.0 - primeira versao publica
1.0.1 - hotfix
1.1.0 - nova mecanica pequena
2.0.0 - mudanca grande ou quebra de compatibilidade
```

Atualize tambem o `version` no `mod.cpp` e no `config.cpp`, se decidir controlar versao por arquivo.

## Observacoes para servidores

Ao atualizar no servidor:

1. Pare o servidor.
2. Atualize `@OPAMilking/Addons`.
3. Atualize a `.bikey` se a chave mudou.
4. Reinicie o servidor.
5. Confirme se o cliente baixou a mesma versao.

Arquivos em `Profiles/OPAMilking` devem ser preservados para manter vacas posicionadas.

