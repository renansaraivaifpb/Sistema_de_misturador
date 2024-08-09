# Documentação do Protótipo de Montagem e Programação
## Introdução
Este documento descreve o processo de montagem e programação para a construção de um protótipo baseado em um esquema elétrico fornecido. O protótipo inclui um sistema de mistura controlado por botões, sensores e atuadores, com a finalidade de realizar e gerenciar ciclos de mistura com feedback em um display LCD.

## Objetivo
Desenvolver um programa em linguagem C para microcontroladores que controla um ciclo de mistura conforme as especificações fornecidas. O sistema deve interagir com botões, sensores e atuadores, além de fornecer informações ao usuário através de um display LCD.

## Especificações de Entrada e Saída
### Entradas Digitais
**Botão C1**: Normalmente aberto. Nível lógico alto quando não pressionado, baixo quando pressionado. (Pino 6)

**Botão C2**: Normalmente aberto. Nível lógico alto quando não pressionado, baixo quando pressionado. (Pino 7)

**Sensor de Nível Vazio**: Nível lógico alto quando acionado (recipiente contém água), baixo quando vazio. (Pino 10)

**Sensor de Nível Cheio**: Nível lógico baixo quando acionado (recipiente cheio), alto quando não cheio. (Pino 11)

### Saídas Digitais
**Bomba 1**: Acionada através dos pinos 2 e 3. Nível lógico alto em um pino e baixo no outro para acionar a bomba; ambos baixos para desativar.

**Bomba 2**: Acionada através dos pinos 4 e 5. Nível lógico alto em um pino e baixo no outro para acionar a bomba; ambos baixos para desativar.

**Misturador**: Nível lógico alto para ligar, baixo para desligar. (Pino 8)

**Válvula Solenoide**: Nível lógico alto para ativar, baixo para desativar. (Pino 9)

## Programa
### Estrutura Geral
O programa é desenvolvido para ser executado em um microcontrolador e deve seguir as seguintes etapas:

### Inicialização:

Configurar pinos de I/O.

Configurar o display LCD.

### Execução do Programa:

Exibir mensagem inicial no display LCD quando o botão C1 for pressionado.

Iniciar ciclo de mistura se o botão C1 for pressionado novamente dentro de 10 segundos.

Cancelar operação e retornar à condição inicial se um botão diferente de C1 for pressionado ou se C1 não for pressionado em 10 segundos.

Durante o ciclo de mistura, exibir o estado atual no display LCD.

Permitir a interrupção do ciclo de mistura com o botão C2.

Informar a execução do esvaziamento do recipiente se C2 for pressionado enquanto não houver ciclo de mistura.
