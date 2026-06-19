# OPA Milking

OPA Milking adiciona vacas estaticas ordenhaveis ao DayZ. O mod foi pensado para servidores multiplayer que querem criar pontos de interesse em fazendas, sem usar IA animal e sem impacto alto no servidor.

## Recursos

- Nova entidade estatica: `OPA_MilkingCow`.
- Acao continua: `Ordenhar vaca`.
- Novo liquido consumivel: `Leite`.
- Persistencia das vacas posicionadas pelo admin.
- Reset automatico da ordenha a cada restart.
- Compatibilidade com recipientes vanilla e modded que aceitam agua.
- Configuracao simples por arquivo JSON no profile do servidor.

## Requisitos

- DayZ Server atualizado.
- Cliente e servidor usando a mesma versao do mod.
- Arquivo `.bikey` instalado na pasta `keys` do servidor, caso `verifySignatures` esteja ativo.

O mod nao depende de CF, COT, VPP ou Expansion para funcionar. Esses mods podem ser usados apenas para spawnar/posicionar vacas durante a administracao.

## Instalacao no servidor

1. Copie a pasta do mod para a raiz do servidor:

```txt
DayZServer/
└── @OPAMilking/
    ├── Addons/
    │   ├── OPAMilking.pbo
    │   └── OPAMilking.pbo.OPAMilking.bisign
    └── mod.cpp
```

2. Copie a chave publica para:

```txt
DayZServer/keys/OPAMilking.bikey
```

3. Adicione o mod na linha de inicializacao:

```txt
-mod=@OPAMilking
```

Se usar outros mods:

```txt
-mod=@CF;@Community-Online-Tools;@VPPAdminTools;@OPAMilking
```

4. Reinicie o servidor.

## Instalacao no cliente

O cliente precisa carregar exatamente o mesmo mod:

```txt
@OPAMilking
```

Em servidor publico, a forma recomendada e publicar o mod na Steam Workshop e adicionar o Workshop ID ao launcher/modpack do servidor.

## Como posicionar vacas

Classe da vaca:

```txt
OPA_MilkingCow
```

Voce pode spawnar e posicionar a vaca com:

- VPP Admin Tools;
- Community Online Tools;
- DayZ Editor;
- init.c;
- ferramentas de mapping que suportem classes customizadas.

A vaca nao e uma IA. Ela e uma entidade estatica otimizada para ficar fixa no mapa.

## Persistencia das vacas

As vacas posicionadas sao salvas automaticamente em:

```txt
Profiles/OPAMilking/cows.json
```

O arquivo salva:

- posicao;
- orientacao.

O estado de ordenha nao e salvo. Isso e intencional: apos cada restart, a vaca volta a poder ser ordenhada.

Tempo de salvamento:

- vaca nova: salva poucos segundos apos ser criada;
- vacas existentes: regravadas periodicamente para manter posicao/orientacao atualizadas.

## Configuracao

Arquivo criado automaticamente no profile:

```txt
Profiles/OPAMilking/settings.json
```

Padrao:

```json
{
    "MilkAmountPerMilking": 1000.0,
    "MilkingsPerCowRestart": 1
}
```

Opcoes:

- `MilkAmountPerMilking`: quantidade maxima de leite gerada por ordenha, em ml.
- `MilkingsPerCowRestart`: quantas vezes cada vaca pode ser ordenhada por restart.

Exemplo:

```json
{
    "MilkAmountPerMilking": 750.0,
    "MilkingsPerCowRestart": 2
}
```

Depois de alterar esse arquivo, reinicie o servidor.

## Nutricao do leite

Os valores nutricionais ficam no `config.cpp` do mod, no topo do arquivo:

```cpp
#define OPA_MILK_FULLNESS_INDEX 1
#define OPA_MILK_ENERGY 500
#define OPA_MILK_WATER 100
#define OPA_MILK_NUTRITIONAL_INDEX 80
#define OPA_MILK_TOXICITY 0
#define OPA_MILK_DIGESTIBILITY 2
```

Para alterar nutricao, edite esses valores, reempacote o PBO, assine novamente e atualize cliente/servidor.

## Regras da ordenha

A acao aparece quando:

- o jogador esta mirando em uma `OPA_MilkingCow`;
- o jogador esta segurando um recipiente valido;
- o recipiente esta vazio ou ja contem leite;
- o recipiente tem espaco livre;
- a vaca ainda tem ordenhas disponiveis no restart atual.

A acao nao aparece quando:

- o recipiente contem agua, gasolina, alcool, desinfetante ou outro liquido;
- o recipiente esta cheio;
- o recipiente nao aceita agua/leite;
- a vaca ja atingiu o limite de ordenhas do restart.

Se a acao for interrompida antes do fim, o recipiente recebe leite proporcional ao tempo de ordenha.

## Compatibilidade

O mod evita sobrescrever classes especificas de recipientes no `config.cpp`.

Em vez disso, ele adiciona suporte a leite em qualquer item que ja aceite agua por mascara de liquido. Isso melhora a compatibilidade com:

- garrafas vanilla;
- cantil;
- panelas;
- caldeirao;
- barril;
- recipientes modded que usam `liquidContainerType` com agua.

O leite nao mistura com outros liquidos. Um recipiente com leite pode receber mais leite, mas um recipiente com outro liquido bloqueia a ordenha.

## Troubleshooting

### A acao de ordenhar nao aparece

Verifique:

- o item na mao e um recipiente de liquido;
- o recipiente esta vazio ou contem leite;
- o recipiente nao esta cheio;
- a vaca e a classe `OPA_MilkingCow`;
- a vaca ainda nao atingiu o limite de ordenhas do restart.

### O leite aparece como ERROR

Possiveis causas:

- cliente sem o mod carregado;
- cliente com versao diferente do servidor;
- PBO antigo no cliente ou servidor;
- conflito com outro mod que altera UI/tooltip de liquidos.

### A vaca sumiu apos restart

Verifique:

```txt
Profiles/OPAMilking/cows.json
```

Se o arquivo estiver vazio, posicione a vaca novamente e aguarde alguns segundos antes de reiniciar o servidor.

### Kick por assinatura

Verifique:

```txt
DayZServer/keys/OPAMilking.bikey
```

E confirme se o cliente esta usando o mesmo PBO publicado no servidor.

## Arquivos importantes

```txt
@OPAMilking/
├── Addons/
│   ├── OPAMilking.pbo
│   └── OPAMilking.pbo.OPAMilking.bisign
└── mod.cpp
```

Profile do servidor:

```txt
Profiles/OPAMilking/
├── cows.json
└── settings.json
```

## Reempacotamento em modpacks

O mod pode ser reempacotado em outro modpack desde que:

- o desenvolvedor original, Opaverick, seja mencionado nos creditos;
- o prefixo interno `OPAMilking` seja preservado ou os paths sejam ajustados corretamente;
- o PBO seja assinado novamente;
- a `.bikey` nova seja instalada no servidor;
- cliente e servidor usem a mesma versao final.

Ao reempacotar, mantenha o nome da classe:

```txt
OPA_MilkingCow
```

Isso evita quebrar vacas ja posicionadas em `cows.json` ou em arquivos de mapping.

## Licenca

OPA Milking e open source sob a licenca MIT.

Veja:

```txt
LICENSE
REPACKING.md
```
