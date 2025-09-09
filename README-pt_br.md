<h1 align="center">Windows Dualsense Unreal Plugin</h1>

<p align="center">
Integre todos os recursos do controle DualSense™ e DualShock 4® da Sony no seu projeto Unreal Engine para Windows.
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
<img src="https://img.shields.io/badge/Unreal%20Engine-5.2+-blue?style=for-the-badge&logo=unrealengine" alt="Unreal Engine 5.x"><br/>
<img src="https://img.shields.io/badge/Platform-Windows-blue?style=for-the-badge&logo=windows" alt="Platform: Windows">
</p>

## 📖 Sobre o Projeto

Este é um plugin para a Unreal Engine que permite a comunicação nativa com os controles DualSense™ (PlayStation 5) e DualShock 4® (PlayStation 4) em sistemas Windows. Ele expõe as funcionalidades únicas de cada controle, como o Feedback Háptico e os Gatilhos Adaptáveis do DualSense, através de uma biblioteca de funções de Blueprint e C++ fácil de usar.

O objetivo é preencher a lacuna deixada pelo suporte genérico a controles no Windows, dando aos desenvolvedores acesso direto às features que tornam os controles da Sony tão imersivos.

## ✨ Funcionalidades

* 🔌 **Conexão Dinâmica (Hot-Swap)**: Detecta a conexão e desconexão de controles automaticamente, mesmo durante o gameplay.
* ⚡ **Otimizado para Multiplayer**: Arquitetura de alta performance com impacto mínimo em jogos de rede.
* 🎮 **Integração Perfeita de Input**: Coexiste perfeitamente com os gerenciadores de input nativos da Unreal (como o Enhanced Input) e outros plugins de gamepad, evitando conflitos entre dispositivos.
* 🎯 **Gatilhos Adaptáveis**: Controle total sobre resistência, efeito e vibração nos gatilhos R2/L2.
* 🔊 **Vibração Baseada em Áudio**: Sincronize o feedback háptico do controle com qualquer fonte de áudio do jogo.
* 💡 **Controle da Lightbar**: Altere a cor do LED do controle dinamicamente.
* 🎤 **Microfone e Áudio**: Gerencie o LED do botão de mudo, o volume do alto-falante e o áudio do headset.
* ⚙️ **Force Feedback**: Integração nativa com o sistema de Force Feedback da Unreal Engine para a vibração padrão dos motores.
* 🎮 **Suporte a Múltiplos Controles**: Gerencie até 4 controles conectados simultaneamente.

## 🎬 Demonstração

Para ver o plugin em ação, assista ao vídeo de uso básico no YouTube.

[Veja vídeo de uso](https://www.youtube.com/watch?v=GrCa5s6acmo)

## 🎮 Projeto de Exemplo: Parrot Game Sample

Para demonstrar o uso prático do **Windows Dualsense Unreal Plugin**, foi desenvolvido um projeto de exemplo utilizando o [*Parrot Game Sample*](https://dev.epicgames.com/documentation/pt-br/unreal-engine/parrot-game-sample-for-unreal-engine) da Epic Games. Este projeto integra funcionalidades-chave do controle DualSense para aprimorar a experiência do jogador.

![Parrot Game Sample](Images/parrot-game-sample-banner.jpg)

### Funcionalidades Implementadas

Neste exemplo, as seguintes funcionalidades do DualSense foram integradas para proporcionar uma jogabilidade mais imersiva:

* **Feedback Visual**: A Lightbar do controle é utilizada para fornecer feedback visual em tempo real ao jogador, alterando cores e efeitos de acordo com eventos do jogo.
* **Vibração (Force Feedback)**: O sistema nativo de Force Feedback foi utilizado para criar efeitos de vibração detalhados, aumentando a imersão em momentos importantes do gameplay.

> 💡 **Dica Pro para Maior Imersão**: Para uma experiência de áudio ainda mais profunda, conecte o controle via USB e utilize um fone de ouvido diretamente nele. Essa configuração também funciona com o controle conectado via Bluetooth, oferecendo maior flexibilidade.

Este projeto serve como um guia prático para desenvolvedores que desejam ver o plugin em ação e aprender como integrar os recursos exclusivos do controle DualSense em seus próprios jogos.

### Onde Baixar

Você pode baixar o projeto de exemplo *Parrot Game Sample* com a integração do DualSense diretamente do nosso repositório no GitHub.


- [**Baixe o projeto de exemplo para o editor aqui**](https://drive.google.com/file/d/198Dko7ZwIX1vz9jw7RtYp4arY9Qp5bJ4/view?usp=drive_link)
- [**Baixe a versão compilada do projeto de exemplo aqui**](https://drive.google.com/file/d/144hM71xZufBe29UzpTNQ1rRe0AYWC-Ka/view?usp=drive_link)


## 🚀 Começando

### Pré-requisitos
* **Unreal Engine**: 5.2 ou superior.
* **Sistema Operacional**: Windows 10 ou 11.
* **Controle**: DualSense™ ou DualShock 4®.

### Instalação Rápida
1.  Acesse a página oficial do plugin na Unreal Engine Marketplace (FAB): [Página do Plugin - FAB](https://www.fab.com/listings/e77a8f1d-8bbe-4673-a5ae-7f222c8c0960)
2.  Clique em **Install** ou **Add to Project** e selecione seu projeto Unreal Engine.
3.  Ative o plugin no Unreal Engine:
    * Abra seu projeto.
    * Vá até `Edit > Plugins`.
    * Procure por **Windows Dualsense Plugin** e ative a caixa de seleção.
4.  Reinicie o Unreal Engine quando solicitado.

## 💻 Uso Básico

O plugin expõe todas as funcionalidades através de bibliotecas de funções estáticas em Blueprint, o que significa que você pode chamar os métodos de qualquer lugar sem precisar adicionar componentes.

### Bibliotecas de Funções em Blueprint
As funções são divididas em duas categorias principais para facilitar o acesso:

* **Sony Gamepad**: Contém métodos de gerenciamento comuns a controles Sony (DualShock e DualSense), como controle de LED, giroscópio, nível de bateria, etc.
* **DualSense Effects**: Contém métodos específicos para os recursos exclusivos do DualSense, como a configuração dos Gatilhos Adaptáveis.

**Exemplo 1: Controle de Luzes (Lightbar e LEDs)**
Você pode alterar dinamicamente a cor da Lightbar do controle ou definir os LEDs indicadores de jogador.

![Controle da Lightbar](Images/Lightbar.png)

**Exemplo 2: Efeitos de Gatilho (Categoria: DualSense Effects)**
Chame as funções diretamente para controlar os recursos do DualSense. Alguns dos efeitos disponíveis incluem:

* 🐎 **Galope (Galloping)**: Simula o trote de um cavalo.
* 💪 **Resistência (Resistance)**: Aplica uma força contrária constante ao pressionar o gatilho.
* 🔫 **Arma (Weapon)**: Cria um efeito de recuo para armas semiautomáticas.
* 🔥 **Tiro Automático (Automatic Gun)**: Vibra rapidamente para simular uma arma automática.

![Efeito de Galope](Images/Galloping.png)
![Efeito de Resistência](Images/Resistance.png)
![Efeito de Arma](Images/Weapon.png)
![Efeito de Tiro Automático](Images/AutomaticGun.png)

### Vibração dos Motores (Force Feedback)
A vibração dos motores do DualSense e do DualShock 4 é gerenciada automaticamente pelo sistema nativo de **Force Feedback** da Unreal Engine. Isso significa que qualquer evento de force feedback acionará a vibração do controle sem necessidade de configuração adicional.

![Force Feedback](Images/VibrationFF.png)

### Gerenciamento de Multiplayer
Para gerenciar múltiplos controles em um cenário local, a abordagem recomendada é utilizar o **GameMode** do seu projeto. No evento `PostLogin`, você pode associar um controle a cada novo jogador que se conectar.

![Configuração Multiplayer no GameMode](Images/Multiplayer.png)

## 🤝 Como Contribuir

Contribuições são bem-vindas! Se você tiver ideias, sugestões ou correções, sinta-se à vontade para abrir uma *Issue* ou enviar um *Pull Request*.

## 📄 Licença

Este projeto é distribuído sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.

## ⭐ Créditos e Agradecimentos

Agradecimento especial a todos que já contribuíram com sugestões, reportaram bugs e ofereceram melhorias de implementação. Um agradecimento também aos desenvolvedores das bibliotecas usadas como inspiração e referência para a criação deste projeto.

Um agradecimento especial também à equipe da Epic Games por criar e disponibilizar o *Parrot Game Sample*, que serviu como uma excelente base para o projeto de exemplo que demonstra as funcionalidades deste plugin.

* [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows)
* [Nielk1 on GIST](https://gist.github.com/Nielk1/6d54cc2c00d2201ccb8c2720ad7538db)
* [DualSenseAPI](https://github.com/BadMagic100/DualSenseAPI/tree/master)
* [flok pydualsense](https://github.com/flok/pydualsense)