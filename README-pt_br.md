<h1 align="center">Windows Dualsense Unreal Plugin</h1>

<p align="center">
Integre todos os recursos do controle DualSense™ da Sony no seu projeto Unreal Engine para Windows.
<br />
<br />
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/issues">Reportar Bug</a>
·
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/issues">Sugerir Funcionalidade</a>
</p>

<p align="center">
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/releases">
<img src="https://img.shields.io/github/v/release/rafaelvaloto/WindowsDualsenseUnreal?style=for-the-badge&logo=github" alt="Latest Release">
</a>
<a href="https://github.com/rafaelvaloto/WindowsDualsenseUnreal/blob/main/LICENSE">
<img src="https://img.shields.io/github/license/rafaelvaloto/WindowsDualsenseUnreal?style=for-the-badge" alt="Licença">
</a>
<img src="https://img.shields.io/badge/Platform-Windows-blue?style=for-the-badge&logo=windows" alt="Platform: Windows"><br/>
<img src="https://img.shields.io/badge/Unreal%20Engine-5.2~5.6-blue?style=for-the-badge&logo=unrealengine" alt="Unreal Engine 5.x">

---

## 📋 Índice

- [Sobre o Projeto](#-sobre-o-projeto)
- [✨ Funcionalidades](#-funcionalidades)
- [🚀 Começando](#-começando)
   - [Pré-requisitos](#pré-requisitos)
   - [Instalação](#instalação)
- [💻 Uso Básico](#-uso-básico)
- [📚 Documentação](#-documentação)
- [🤝 Como Contribuir](#-como-contribuir)
- [📄 Licença](#-licença)


## 📖 Sobre o Projeto

Este é um plugin para a Unreal Engine que permite a comunicação nativa com os controles DualSense™ (PlayStation 5) e DualShock 4® (PlayStation 4) em sistemas Windows. Ele expõe as funcionalidades únicas de cada controle, como o Feedback Háptico e os Gatrilhos Adaptáveis do DualSense, através de uma biblioteca de funções de Blueprint e C++ fácil de usar.

O objetivo é preencher a lacuna deixada pelo suporte genérico a controles no Windows, dando aos desenvolvedores acesso direto às features que tornam os controles da Sony tão imersivos.

---

## 🚀 Começando

### Pré-requisitos
🚀 Unreal Engine: 5.2 ou superior.

🪟 Sistema Operacional: Windows 10 ou 11.

🎮 Controle: DualSense™ ou DualShock 4®.

### Instalação Rápida
   1. Acesse a página oficial do plugin no FAB: Página do Plugin - FAB


   2. Clique em Install ou Add to Project Selecione seu projeto Unreal Engine.


   3. Ative o plugin no Unreal Engine:
      - Abra seu projeto.
      - Vá até ```Edit > Plugins```.
      - Procure por Windows DualSense Plugin e ative a caixa de seleção.

Reinicie o Unreal Engine quando solicitado.

## ✨ Funcionalidades

✅ Feedback Háptico (Haptic Feedback): Controle total sobre a vibração de baixa e alta frequência.

🔊 Vibração Baseada em Áudio: Sincronize o feedback háptico do controle com qualquer fonte de áudio do jogo em tempo real.

🎯 Gatilhos Adaptáveis (Adaptive Triggers): Configure resistência, efeito e vibração nos gatilhos R2/L2.

🎚️ Controle de Intensidade: Ajuste preciso da força dos gatilhos adaptáveis e da intensidade da vibração geral.

💡 Luz do Touchpad (Lightbar): Altere a cor do LED do controle dinamicamente.

🎤 LED do Microfone (Mute Button): Controle o estado do LED do botão de mudo.

🔈 Controle de Áudio: Gerencie o volume do alto-falante integrado e do microfone do headset conectado.

🎮 Suporte a Múltiplos Controles: Gerencia até 4 controles DualSense conectados simultaneamente.

---

## 🎬 Demonstração

[Veja vídeo de uso](https://www.youtube.com/watch?v=GrCa5s6acmo)

---

## 💻 Uso Básico

O plugin expõe todas as funcionalidades através de bibliotecas de funções estáticas em Blueprint, o que significa que você pode chamar os métodos de qualquer lugar sem precisar adicionar componentes.

Como Usar as Funções em Blueprint
As funções são divididas em duas categorias principais para facilitar o acesso:

Sony Gamepad: Contém métodos de gerenciamento comuns a controles Sony (DualShock e DualSense), como conexão, controle de LED, giroscópio, etc.

DualSense Effects: Contém métodos específicos para os recursos exclusivos do DualSense, como a configuração dos Gatrilhos Adaptáveis.

Exemplo 1: Gerenciamento de Conexão (Categoria: Sony Gamepad)
Você pode verificar se um controle está conectado e gerenciar sua conexão diretamente.

![Blueprint](Images/DS5_DS4.png)

💡 Luz do Touchpad (Lightbar): Altere a cor do LED do controle dinamicamente.

![Blueprint](Images/Lightbar.png)

Exemplo 2: Chame as funções diretamente para controlar os recursos do DualSense. Alguns dos efeitos de gatilho disponíveis incluem:

- 🐎 Galope (Galloping): Simula o trote de um cavalo.

- 💪 Resistência (Resistance): Aplica uma força contrária constante ao pressionar o gatilho.

- 🔫🔥 Tiro Automático (AutomaticGun): Vibra rapidamente para simular uma arma automática.

- 🔫 Tiro Semiautomático (SemiAutomaticGun): Cria um recuo rápido para simular um único disparo.

![Unreal Editor Blueprint](Images/Galloping.png)

![Unreal Editor Blueprint](Images/Resistance.png)

![Unreal Editor Blueprint](Images/AutomaticGun.png)

![Unreal Editor Blueprint](Images/Weapon.png)



## ⚙️ Vibração dos Motores (Force Feedback)
A vibração dos motores do DualSense e DualShock 4 é ativada automaticamente pelo sistema de force feedback do Unreal Engine.
Isso significa que qualquer evento de force feedback já suportado nativamente pelo Unreal (Blueprints ou C++) irá acionar a vibração dos motores dos controles, sem necessidade de configuração adicional.

Como usar:
Basta adicionar eventos de force feedback no seu projeto Unreal normalmente.
O plugin garante compatibilidade dos motores de vibração dos controles PlayStation com essas ações.


![Unreal Editor Blueprint](Images/VibrationFF.png)

![Unreal Editor Blueprint](Images/TriggerReduce.png)

## 🎮 Multiplayer
Para gerenciar múltiplos controles em um cenário local, a abordagem recomendada é utilizar o GameMode.

Exemplo: Multiplayer com 4 Jogadores
No seu GameMode, utilize o evento PostLogin para criar e configurar até 4 jogadores. Cada vez que um jogador conecta, um novo player é adicionado e associado ao respectivo controle.

![Unreal Editor Blueprint](Images/Multiplayer.png)

---

## 🤝 Como Contribuir

Contribuições são bem-vindas! Se você tiver ideias, sugestões ou correções, sinta-se à vontade para abrir uma *Issue* ou enviar um *Pull Request*.

---

## 📄 Licença

Este projeto é distribuído sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.

---

## ⭐ Créditos e Agradecimentos

Agradecimento especial a todos que já contribuíram com sugestões, reportaram bugs e ofereceram melhorias de implementação. Um agradecimento também aos desenvolvedores das bibliotecas usadas como inspiração e referência para a criação deste projeto.

- [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows)
- [Nielk1 on GIST](https://gist.github.com/Nielk1/6d54cc2c00d2201ccb8c2720ad7538db)
- [DualSenseAPI](https://github.com/BadMagic100/DualSenseAPI/tree/master)
- [flok pydualsense](https://github.com/flok/pydualsense)


