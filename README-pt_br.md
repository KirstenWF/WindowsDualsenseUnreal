<h1 align="center">Dualsense Unreal Plugin</h1>

<p align="center">
Integrar todos os recursos dos controles DualSense™ e DualShock 4® da Sony em seu projeto Unreal Engine.
<br />
<br />
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/issues">Relatar Bug</a>
·
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/issues">Sugerir Recurso
</a>
·
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/wiki">Documentação
</a>
</p>

<p align="center">
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/releases">
    <img src="https://img.shields.io/github/v/release/rafaelvaloto/Unreal-Dualsense?style=for-the-badge&logo=github" alt="Última Versão">
</a>
<a href="https://github.com/rafaelvaloto/Unreal-Dualsense/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/rafaelvaloto/Unreal-Dualsense?style=for-the-badge" alt="Licença">
</a>
<img src="https://img.shields.io/badge/Unreal%20Engine-5.2+-blue?style=for-the-badge&logo=unrealengine" alt="Unreal Engine 5.x">
<br/>
<img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Plataforma: Windows">
<img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black" alt="Plataforma: Linux">
<img src="https://img.shields.io/badge/macOS-000000?style=for-the-badge&logo=apple&logoColor=white" alt="Plataforma: macOS">
<img src="https://img.shields.io/badge/PlayStation-0070D1?style=for-the-badge&logo=playstation&logoColor=white" alt="Plataforma: PlayStation">
</p>

<p align="center">
  <a href="#-primeiros-passos">Primeiros Passos</a> •
  <a href="#-uso-básico">Uso Básico</a> •
  <a href="#-projeto-de-exemplo-arena-shooter-ue-56">Projeto de Exemplo</a> •
<a href="#-prototipagem-de-hápticos-ao-vivo-console-para-blueprint">Prototipagem</a> •
  <a href="#%EF%B8%8F-estendendo-para-outras-plataformas-ex-playstation">Estendendo</a> •
<a href="#-injetando-lógica-de-dispositivo-personalizada-custom-devicemanager">Injetando Dispositivo Personalizado</a>
  <br>
  <br>
  <a href="#-mantenedores-principais">💖 Por que seu Estúdio deve patrocinar este projeto? 💖</a>
	<br>
</p>

> [!IMPORTANT]
> **v2 já está disponível!**
> 
> Agora você pode estender o plugin para implementar lógica personalizada para o **Giroscópio, Acelerômetro e Touchpad**, ou até mesmo integrar assets nativos da Unreal Engine. 
> - **Arquitetura Extensível:** O plugin apresenta um filtro IMU pré-configurado que pode ser totalmente sobrescrito.
> - **Implementação Personalizada:** Ajuste o comportamento do dispositivo às necessidades do seu projeto na [Seção de Customização](#-injetando-lógica-de-dispositivo-personalizada-custom-devicemanager).
> 
> 🔄 **Atualizando da v1.x?** Por favor, leia nosso [Guia de Migração](https://github.com/rafaelvaloto/Unreal-Dualsense/wiki/Migration-Guide:-Unreal%E2%80%90Dualsense-v1.x-to-v2.0).

## 📖 Sobre o Projeto

Construído com uma arquitetura multiplataforma, este plugin para Unreal Engine oferece uma solução unificada para integrar os controles DualSense™ (PlayStation 5) e DualShock 4® (PlayStation 4). Ele oferece suporte nativo no PC, sendo arquitetado para facilitar a compilação em outras plataformas da Unreal Engine, incluindo PlayStation®. Este asset fornece acesso direto via API ao conjunto completo de recursos de cada controle, incluindo o revolucionário Haptic Feedback (Resposta Háptica) e Adaptive Triggers (Gatilhos Adaptáveis) do DualSense™. Todos os recursos estão expostos através de uma biblioteca de funções limpa e bem documentada para Blueprint e C++.

Projetado para preencher a lacuna deixada pelo suporte genérico a controles, este asset capacita os desenvolvedores a implementar o feedback imersivo e de alta fidelidade que torna os controles da Sony únicos.

## ✨ Recursos
* 🏗️ **Arquitetura Multiplataforma Extensível**: Seu design flexível permite a compilação em todas as plataformas da Unreal Engine. Suportar novo hardware é tão simples quanto implementar a interface de conexão.
* 🔌 **Conexão Dinâmica (Hot-Swap)**: Detecta automaticamente a conexão e desconexão do controle, mesmo durante o jogo.
* ⚡ **Otimizado para Multiplayer**: Arquitetura de alto desempenho com impacto mínimo em jogos de rede.
* 🎮 **Integração de Input Sem Costura**: Coexiste perfeitamente com os gerenciadores de input nativos da Unreal Engine (como Enhanced Input) e outros plugins de gamepad, evitando conflitos entre dispositivos.
* 🎧 **Haptics de Áudio (USB e Sem Fio)**: Resposta háptica baseada no áudio do jogo.
* 🎯 **Gatilhos Adaptáveis**: Controle total sobre resistência, efeito e vibração nos gatilhos R2/L2.
* 💡 **Controle de Lightbar**: Altere dinamicamente a cor do LED do controle.
* 🎤 **Detecção Inteligente de Mudo** — Controle automático do LED de mudo, sem necessidade de codificação.
* ⚙️ **Force Feedback**: Integração nativa com o sistema de Force Feedback da Unreal Engine para vibração de motor padrão.
* 🎮 **Suporte a Múltiplos Controles**: Gerencie até 4 controles simultaneamente.

## 🚀 Primeiros Passos

### Pré-requisitos

* **Unreal Engine**: 5.2 ou superior (O plugin utiliza recursos de C++20).
* **Sistema Operacional**: Windows 10 ou 11.
* **Controle**: DualSense™ ou DualShock 4®.

### Instalação Rápida

1.  Acesse a página oficial do plugin no Unreal Engine Marketplace (FAB): [Página do Plugin - FAB](https://www.fab.com/listings/e77a8f1d-8bbe-4673-a5ae-7f222c8c0960)
2.  Clique em **Install** ou **Add to Project** e selecione seu projeto Unreal Engine.
3.  Ative o plugin na Unreal Engine:
    * Abra seu projeto.
    * Vá em `Edit > Plugins`.
    * Pesquise por **Windows Dualsense Plugin** e marque a caixa.
4.  Reinicie a Unreal Engine quando solicitado.

### Instalação Manual

Para garantir que o plugin compile corretamente na Unreal Engine, você deve configurar o submódulo [GamepadCore](https://github.com/rafaelvaloto/GamepadCore_).

Por favor, execute os seguintes comandos no seu terminal (Git Bash, PowerShell ou CMD):
```bash
# Clone the repository at version (tag)
git clone -b [tag] --single-branch https://github.com/rafaelvaloto/Unreal-Dualsense.git
```
```bash
# Clone the repository at master branch
git clone https://github.com/rafaelvaloto/Unreal-Dualsense.git

# Enter the repository folder
cd Unreal-Dualsense

# Init the submodule to the latest version
git submodule update --init

# Init the submodule miniaudio
git -C Source/WindowsDualsense_ds5w/Private/GamepadCore submodule update --init Libs/miniaudio
```

## 💻 Uso Básico

O plugin expõe todas as funcionalidades através de bibliotecas de funções estáticas em Blueprint, o que significa que você pode chamar métodos de qualquer lugar sem a necessidade de adicionar componentes.

> [!IMPORTANT]
> **Novidade na v2: Nó Update Output**
>
> Na versão 2.x, após configurar qualquer efeito de controle (como Lightbar, Gatilhos Adaptáveis ou LED do Mic), você **deve** chamar o nó **Update Output** para aplicar essas alterações ao controle. Essa otimização permite agrupar várias alterações de efeitos e enviá-las em uma única atualização.
>
> **Exemplo:** Set Lightbar Color ⮕ Set Weapon Effect (Trigger) ⮕ **Update Output**.

### Bibliotecas de Funções Blueprint

As funções são divididas em duas categorias principais para facilitar o acesso:

* **Sony Gamepad**: Contém métodos de gerenciamento comuns aos controles da Sony (DualShock e DualSense), como controle de LED, giroscópio, nível de bateria, etc.
* **DualSense Effects**: Contém métodos específicos para recursos exclusivos do DualSense, como a configuração dos Gatilhos Adaptáveis.

Chame as funções diretamente para controlar os recursos do DualSense. Alguns efeitos disponíveis incluem:

* 🐎 **Galloping**: Simula o trote de um cavalo.
* 💪 **Resistance**: Aplica uma força oposta constante ao pressionar o gatilho.
* 🔫 **Weapon**: Cria um efeito de recuo para armas semiautomáticas.
* 🔥 **Automatic Gun**: Vibra rapidamente para simular uma arma automática.

### 📚 Para a documentação completa, consulte a **[Wiki](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki)**.


## 🎮 Projeto de Exemplo: Arena Shooter UE 5.6

Para demonstrar o uso prático do **Dualsense Unreal Plugin**, foi desenvolvido um projeto de exemplo utilizando o [*template Arena Shooter*](https://www.unrealengine.com/marketplace/en-US/product/arena-shooter-template) e atualizado para a Unreal Engine 5.6. Este projeto integra os principais recursos do controle DualSense para aprimorar a experiência do jogador.

🎬 **Demo de Giroscópio:** Veja a nova mecânica de mira em ação no **[YouTube](https://youtu.be/r_NhoODeELU?si=IdqRFx8_aimeIToD)**.

![Arena Shooter UE 5.6](Images/ArenaBanner.jpg)

### Recursos Implementados

Neste exemplo, as seguintes funcionalidades do DualSense foram integradas para proporcionar uma experiência de jogo mais imersiva:

* 🔫 **Automatic Gun**: Experimente o disparo totalmente automático com resposta háptica apropriada, simulando o recuo e a vibração realistas da arma.
* 🎯 **Semi-Automatic Gun**: Envolva-se em tiros precisos com armas semiautomáticas, onde cada tiro oferece sensações hápticas distintas e resistência no gatilho.
* 💥 **Vibração ao Ser Atingido e Feedback Visual**: Sinta o impacto! O controle vibra dinamicamente quando o jogador sofre dano, complementado por feedback visual na tela para maior imersão e percepção imediata.
* 💡 **Mudança de Cor do LED ao Ser Atingido**: O LED do controle DualSense muda de cor dinamicamente quando o jogador é atingido, oferecendo uma dica visual imediata e intuitiva do dano recebido, melhorando a consciência situacional.

### Onde Baixar

Você pode baixar o *Arena Shooter UE 5.6* com a integração DualSense diretamente pelo link abaixo.

- [**Baixe o projeto de exemplo v2.0.0 para o editor aqui**](https://drive.google.com/file/d/11iUQuWwA4zkFI_eP0roYbTDh0ss8614m/view?usp=drive_link)

### 🎓 Tutorial Mão na Massa

Criamos um tutorial detalhado passo a passo que detalha toda a implementação dentro do projeto de exemplo. É o guia perfeito para você começar.

* 🎯 **Mira com Giroscópio**: Um tutorial completo sobre como implementar um sistema de mira por giroscópio preciso e responsivo (ADS) usando o template Arena Shooter.
  ➡️ **[[Leia o Tutorial de Mira com Giroscópio]](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/Example-Project:-Gyroscope-Aiming)**

* 🔫 **Arena Shooter**: Um exemplo usando o template Arena Shooter que implementa gatilhos adaptáveis para armas automáticas/semiautomáticas e resposta háptica para dano ao jogador.
  ➡️ **[[Leia o Tutorial do Arena Shooter]](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/Example-Project:-Arena-Shooter-Tutorial)**


## 🎮 Projeto de Exemplo: Haptics de Áudio (USB e Sem Fio)

Leve sua imersão para o próximo nível! Esta atualização aprimora o recurso avançado de Audio Haptics, permitindo que ele funcione perfeitamente via USB e sem fio via Bluetooth (anteriormente apenas USB). Este novo projeto de exemplo demonstra como aproveitar esse poder, apresentando uma implementação completa para resposta háptica em tempo real baseada no áudio do jogo.

![Audio Haptics (USB & Wireless)](Images/audio-haptics-bluetooth-and-usb-ds.png)

### Recursos Implementados

Este projeto de exemplo serve como um guia prático e inclui:

* **🎧 Listener de Submix Flexível:** Aprenda como registrar um listener (RegisterSubmixForDevice) em um Sound Submix da Unreal Engine que processa áudio para conexões com e sem fio.

* **⚡️ Processamento de Áudio em Tempo Real:** Capturamos os dados de áudio diretamente do submix, fazemos o re-sample e os enviamos para o controle DualSense através da sua conexão ativa (USB ou Bluetooth) em tempo real.

* **🎛️ Widget de Menu Haptic FX:** Uma nova interface de exemplo (UMG Widget) é fornecida, permitindo que você:

Selecione diferentes Sound Classes para serem roteadas para o sistema háptico.

Reproduza vários "álbuns de reprodução" (conjuntos de Sound Cues) para testar e sentir uma grande variedade de efeitos hápticos baseados em diferentes sons.

🛠️ Novas Ferramentas de Desenvolvedor/Debug: Para usuários avançados, novos comandos de console foram adicionados para testar e ajustar as vibrações e frequências do gatilho diretamente em conexões sem fio e com fio (ex: ds.SetAudioLR).

- [**Baixe o projeto de exemplo para o editor aqui**](https://drive.google.com/file/d/1lGUBfz42gnh2bSIJTptC_LqOdFHsPuSi/view?usp=sharing)


## 🚀 Prototipagem de Hápticos ao Vivo (Console para Blueprint)

Agora você pode descobrir, testar e implementar efeitos de gatilho avançados com um novo fluxo de trabalho altamente eficiente.

**1. Teste ao Vivo no Console:** Ajuste os efeitos dos gatilhos adaptáveis diretamente no console da Unreal Engine. Esta é a maneira mais rápida de prototipar e depurar sensações hápticas sem recompilar. Use os comandos `ds.SetTrigL` e `ds.SetTrigR` para enviar arrays HEX brutos de 10 bytes até descobrir o efeito perfeito.

**2. Armazene e Reutilize:** Depois de ter seus valores HEX ideais, não os escreva diretamente no código (hard-code)! Armazene-os em uma **Data Table** reutilizável para ser chamada de qualquer Blueprint usando o nó `Custom Trigger`.

Este fluxo de trabalho completo — desde a descoberta ao vivo no console até a implementação limpa em Blueprint — é abordado em nossos novos guias da Wiki:

* ➡️ **[Página da Wiki: Comandos de Console e Referência HEX](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/%F0%9F%8E%AE-DualSense-Trigger-Effects:-Console-Commands-&-HEX-Reference)**
    * (Aprenda a *encontrar e testar* efeitos usando o console)
* ➡️ **[Página da Wiki: Tutorial: Criando uma Data Table de Efeito de Gatilho Reutilizável](https://github.com/rafaelvaloto/WindowsDualsenseUnreal/wiki/%F0%9F%8E%93-Tutorial:-Creating-a-Reusable-Trigger-Effect-Data-Table)**
    * (Aprenda a *armazenar e usar* seus efeitos em Blueprints)


## 💉 Injetando Lógica de Dispositivo Personalizada (Custom DeviceManager)
Desde a versão 2.0.0, você também pode injetar uma implementação personalizada do `DeviceManager`. Isso é útil se você deseja implementar seu próprio buffer de entrada, mapeamento de botões personalizado ou lógica háptica especializada sem modificar o código-fonte do plugin.

> [!TIP]
> Para garantir que sua implementação personalizada funcione com assets nativos da Unreal Engine (como **Haptic Feedback Effects**, **Force Feedback Assets** e **Device Properties**), sua classe deve implementar ou sobrescrever corretamente os métodos de `IInputDevice` e `IHapticDevice`.

### Interfaces Necessárias para Assets Nativos

Se você deseja que seu gerenciador personalizado suporte recursos nativos da Unreal, certifique-se de que ele implemente/sobrescreva:

*   **`IHapticDevice`**: assets hápticos.
    *   `SetHapticFeedbackValues`: Processa valores de frequência e amplitude dos assets.
    *   `GetHapticFrequencyRange`: Determina a faixa de frequência válida suportada pelo dispositivo.
    *   `GetHapticAmplitudeScale`: Retorna o fator de escala para o mapeamento de amplitude.
*   **`IInputDevice`**: Necessário para vibração padrão, cor da luz e propriedades.
    *   `SetChannelValues` / `SetChannelValue`: Essencial para assets `UForceFeedbackEffect`.
    *   `SetLightColor` / `ResetLightColor`: Controla o LED do controle.
    *   `SetDeviceProperty`: Gerencia `UInputDeviceProperty` (ex: Gatilhos Adaptáveis via sistema Unreal 5.1+).
    *   `GetHapticDevice`: Retorna a interface `IHapticDevice*` (geralmente `return this;`).
    *   `IsGamepadAttached`: Retorna se o dispositivo está conectado no momento.

### Exemplo de implementação personalizada:

1. Crie sua classe personalizada herdando de `DeviceManager`:
```cpp
// Preencha o aviso de direitos autorais na página de Configurações do Projeto.

#include "MyProject.h"

#include "DeviceManager.h"
#include "Modules/ModuleManager.h"
#include "WindowsDualsense_ds5w.h"

class FMyCustomDeviceManager : public DeviceManager
{
public:
    using DeviceManager::DeviceManager;
	/** * Map to track the previous frame's touch state per DeviceID.
	 * Marked as 'mutable' so it can be updated within const methods.
	 */
	mutable TMap<int32, bool> DeviceTouchStates;
    
	virtual void TouchpadImpl(FDeviceContext* Context, FInputContext& FrameInput, const FPlatformUserId UserId,
	                          const FInputDeviceId InputDeviceId, float DeltaTime) const override
	{
	    // --- 1. Basic Touch Mapping (Unreal Message Handler) ---
	    if (Context->bEnableTouch)
	    {
	        bool& bWasTouchDown = DeviceTouchStates.FindOrAdd(InputDeviceId.GetId(), false);
	
	        if (FrameInput.bIsTouching && !bWasTouchDown)
	        {
	            MessageHandler->OnTouchStarted(nullptr, FrameInput.TouchPosition, 1.0f, FrameInput.TouchId, UserId, InputDeviceId);
	        }
	        else if (FrameInput.bIsTouching && bWasTouchDown)
	        {
	            MessageHandler->OnTouchMoved(FrameInput.TouchPosition, 1.0f, FrameInput.TouchId, UserId, InputDeviceId);
	        }
	        else if (!FrameInput.bIsTouching && bWasTouchDown)
	        {
	            MessageHandler->OnTouchEnded(FrameInput.TouchPosition, FrameInput.TouchId, UserId, InputDeviceId);
	        }
	
	        bWasTouchDown = FrameInput.bIsTouching;
	    }
	
	    // --- 2. Gesture Mapping (Two-Finger Scroll) ---
	    if (Context->bEnableGesture)
	    {
	        // Check if exactly 2 fingers are touching the pad
	        if (FrameInput.bIsTouching && FrameInput.TouchFingerCount == 2)
	        {
	            MessageHandler->OnTouchGesture(
	                EGestureEvent::Scroll,
	                ScrollDelta,
	                0.0f,   /* Value / Total movement if needed */
	                false   /* IsInverted */
	            );
	        }
	    }
	}

};

```

2. Registre sua factory personalizada em seu Game Module:
```cpp
class FMyProject : public FDefaultGameModuleImpl {
public:
    virtual bool IsGameModule() const override { return true; }

    virtual void StartupModule() override {
        FWindowsDualsense_ds5wModule::SetCustomInputDeviceFactory([](const TSharedRef<FGenericApplicationMessageHandler>& InHandler)
        {
            UE_LOG(LogTemp, Log, TEXT("MyProject Game Module: Init FMyCustomDeviceManager."));
            return MakeShared<FMyCustomDeviceManager>(InHandler);
        });
    }
};

IMPLEMENT_PRIMARY_GAME_MODULE( FMyProject, MyProject, "MyProject" );
```


3. Configuração de Build
   Certifique-se de que o `Build.cs` do seu projeto inclua o módulo do plugin e habilite o suporte a C++20:
```csharp
public class NewDeveloper : ModuleRules
{
    public NewDeveloper(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // Necessário para Concepts e arquitetura Policy-Based
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { 
            ...
            "WindowsDualsense_ds5w" 
        });
    }
}
```

## 🛠️ Estendendo para Outras Plataformas (ex: PlayStation)
O plugin possui uma arquitetura desacoplada usando Policy-Based Design, permitindo que os desenvolvedores integrem outros SDKs de plataforma (como o SDK oficial da Sony PlayStation®) ou wrappers HID personalizados diretamente de seu Projeto de Jogo.

A principal vantagem é que você não precisa modificar o código-fonte do plugin. Você pode injetar sua implementação durante a inicialização do aplicativo.

1. Implementação via Hardware Policy
   A comunicação de hardware de baixo nível é abstraída através de um sistema de Template-Policy. Para adicionar uma nova plataforma, você cria uma struct C++ simples em seu projeto que implementa os métodos de hardware necessários (Read, Write, Detect, etc.).
```cpp
#pragma once
#include "CoreMinimal.h"

namespace SonyPlatformPolicy 
{
    struct FSonyHardware 
    {
        FSonyHardware() = default;

        // Implementação dos métodos exigidos pela Hardware Policy
        void Read(FDeviceContext* Context) { /* Seu SDK Read */ }
        void Write(FDeviceContext* Context) { /* Seu SDK Write */ }
        void Detect(TArray<FDeviceContext>& Devices) { /* Seu SDK Detect */ }
        bool CreateHandle(FDeviceContext* Context) { return true; }
        void InvalidateHandle(FDeviceContext* Context) { /* Limpeza */ }
        void ProcessAudioHaptic(FDeviceContext* Context) { /* Lógica de Haptics */ }
    };
}
```
2. Injeção via Game Module
      Em vez de modificar um singleton dentro do plugin, você "injeta" sua plataforma de hardware personalizada durante a inicialização do seu Game Module. Isso garante que sua lógica específica do projeto tenha precedência sobre a implementação HID padrão.
   Exemplo de Implementação em seu Game Module (NewDeveloper.cpp):
```cpp
#include "NewDeveloper.h"
#include "Modules/ModuleManager.h"
#include "Implementations/Platforms/Others/GamepadHardwareBridge.h"
#include "Platforms/SonyPlatformPolicy.h"
#include <memory>

class FNewDeveloper : public FDefaultGameModuleImpl {
public:
    virtual bool IsGameModule() const override { return true; }

    virtual void StartupModule() override {
        // Injetando a plataforma de hardware personalizada no Plugin Bridge
        auto CustomPlatform = std::make_unique<SonyPlatformPolicy::FSonyHardware>();
        FGamepadHardwareBridge::InjectHardwarePlatform(std::move(CustomPlatform));
        
        UE_LOG(LogTemp, Log, TEXT("NewDeveloper Game Module: Custom Hardware Policy Injected."));
    }
};

IMPLEMENT_PRIMARY_GAME_MODULE(FNewDeveloper, NewDeveloper, "NewDeveloper");
```

3. Configuração de Build
   Certifique-se de que o `Build.cs` do seu projeto inclua o módulo do plugin e habilite o suporte a C++20:
```csharp
public class NewDeveloper : ModuleRules
{
    public NewDeveloper(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        
        // Necessário para Concepts e arquitetura Policy-Based
        CppStandard = CppStandardVersion.Cpp20;

        PublicDependencyModuleNames.AddRange(new string[] { 
            ...
            "WindowsDualsense_ds5w" 
        });

        // Adicione as bibliotecas do seu SDK personalizado aqui
        // PublicSystemLibraries.Add("MySDK.lib");
    }
}
```
---
## 🤝 Mantenedores Principais

Este plugin é mantido ativamente por:

* **[Rafael Valoto](https://github.com/rafaelvaloto)** – Autor original e desenvolvedor líder
* **[Marat Radchenko (@slonopotamus)](https://github.com/slonopotamus)** – Engenheiro Sênior (CTO), sistemas de build de nível empresarial, CI/CD e compilação multiplataforma

---
## 💖 Patrocine o Projeto
<div align="center">

<a href="https://github.com/sponsors/rafaelvaloto">
  <img src="https://img.shields.io/static/v1?label=Sponsor&message=%E2%9D%A4&logo=GitHub&color=ea4aaa&style=for-the-badge" height="45" alt="Sponsor Button">
</a>

<p><b>Ajude a sustentar ferramentas de código aberto "AAA" para desenvolvedores.</b></p>

</div>
<br>
Esta é uma ferramenta de nível profissional e código aberto, construída para levar recursos avançados de controle a todos os desenvolvedores — desde indies até estúdios AAA.

---

## 📉 Economia Massiva de Engenharia
Manter implementações de hardware separadas para diferentes plataformas é um "ralo" de orçamento. Este plugin elimina esse custo:
* **Código Unificado:** Escreva sua lógica de haptics e triggers uma única vez. Um único caminho de código para todas as plataformas.
* **Abstração de Hardware:** Economize **centenas de horas de engenharia** que seriam gastas em manutenção de baixo nível (HID) e casos isolados de firmware.
* **Manutenção Zero:** Quando uma plataforma atualiza seu SDK, você atualiza apenas a "ponte", não o sistema de input inteiro do seu jogo.

---

### 🔒 Arquitetura Segura para NDAs (Compatível com SDK PlayStation)
**Injete SDKs licenciados/proprietários sem modificar o código-fonte do plugin.**

O plugin utiliza um "Policy-Based Design" que atua como uma ponte: seu código de plataforma coberto por NDA permanece no **seu projeto**.
- ✅ Sem violações de NDA ou conflitos de licença.
- ✅ SDKs oficiais da Sony podem ser integrados com segurança.
- ✅ Seu código proprietário permanece sob seu controle total.

### Por que estúdios escolhem este Plugin:
- **🌍 API Cross-Platform** – Lógica de input unificada para Windows, Linux, macOS e PlayStation.
- **⚡ Pronto para Produção** – Arquitetura C++20 com zero overhead, usado em títulos lançados.
- **🚀 Desenvolvimento Ágil** – Prototipagem de haptics em tempo real via comandos de console.

---

## 📄 Licença

Este projeto é distribuído sob a Licença MIT. Veja o arquivo `LICENSE` para mais detalhes.

## 🤝 Como Contribuir

Contribuições são bem-vindas! Se você tem ideias, sugestões ou correções de bugs, sinta-se à vontade para abrir uma *Issue* ou enviar um *Pull Request*.

---
## ⭐ Créditos e Agradecimentos

A base deste plugin foi construída sobre a pesquisa e o código de vários projetos incríveis na comunidade:

* [DualSense on Windows API](https://github.com/Ohjurot/DualSense-Windows) - Lógica de implementação inicial do DS5.
* [Nielk1 on GIST](https://gist.github.com/Nielk1/6d54cc2c00d2201ccb8c2720ad7538db) - Estruturas de relatório HID.
* [DualSenseAPI](https://github.com/BadMagic100/DualSenseAPI/tree/master) - Referências de comunicação de hardware.
* [flok pydualsense](https://github.com/flok/pydualsense) - Pesquisa de relatórios de recursos.
* [SAxense](https://github.com/egormanga/SAxense) - Base para Audio Haptics via Bluetooth.
* [miniaudio](https://github.com/mackron/miniaudio) - Biblioteca de reprodução e conversão de áudio.
* [Ryochan7/DS4Windows](https://github.com/Ryochan7/DS4Windows) - Padrão da indústria para DualShock/DualSense no Windows.
* [linux/drivers/hid/hid-playstation.c](https://github.com/torvalds/linux/blob/master/drivers/hid/hid-playstation.c#L1709) - Referência para calibração, giroscópio e padrões de driver Linux.

Agradecimentos especiais aos membros da comunidade que ajudaram a melhorar este plugin:

* **[yncat](https://github.com/yncat)**: Pela extensa pesquisa e lógica de implementação referente ao **USB Audio Haptics**, que foi crucial para suportar haptics de alta fidelidade via USB ([Issue #105](https://github.com/rafaelvaloto/Unreal-Dualsense/issues/105)).

Um agradecimento especial à equipe da Unreal Engine por fornecer os templates Arena Shooter, que serviram como uma excelente base para o projeto de exemplo que demonstra os recursos deste plugin.

---

## ⚖️ Isenção de Responsabilidade e Marcas Registradas

Este software é um projeto independente e não oficial. Ele **não** é afiliado, associado, autorizado, endossado por, ou de qualquer forma oficialmente conectado à Sony Interactive Entertainment Inc., Microsoft Corporation, Apple Inc., Epic Games, Unity Technologies, ao projeto Godot Engine ou à Open 3D Foundation.

**As marcas registradas pertencem aos seus respectivos proprietários:**

* **Sony:** "PlayStation", "PlayStation Family Mark", "PS5 logo", "PS5", "DualSense" e "DualShock" são marcas registradas ou marcas comerciais da Sony Interactive Entertainment Inc. "SONY" é uma marca registrada da Sony Corporation.
* **Microsoft:** "Windows" e "Xbox" são marcas registradas da Microsoft Corporation.
* **Apple:** "Mac" e "macOS" são marcas registradas da Apple Inc.
* **Linux:** "Linux" é a marca registrada de Linus Torvalds nos EUA e em outros países.
* **Epic Games:** "Unreal" e "Unreal Engine" são marcas comerciais ou marcas registradas da Epic Games, Inc. nos Estados Unidos da América e em outros lugares.
* **Unity:** "Unity", os logotipos da Unity e outras marcas comerciais da Unity são marcas comerciais ou marcas registradas da Unity Technologies ou de suas afiliadas nos EUA e em outros lugares.
* **Godot:** "Godot" e o logotipo da Godot são marcas comerciais do projeto Godot Engine.
* **O3DE:** "O3DE" e o logotipo da O3DE são marcas comerciais da Open 3D Foundation.
