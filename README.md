# Banker Tycoon

**Banker Tycoon** is an educational loan-assessment game developed in **C++** using **raylib**, **Dear ImGui**, and **SQLite**.

The player acts as a bank employee and must examine loan applications, analyse the available financial documents, and decide whether each application should be approved or rejected.

The project was created as part of a bachelor's thesis about the use of gamification to explain **information asymmetry in bank–business relationships**.

## Gameplay Demo

The following video shows a complete gameplay cycle, from the analysis
of loan applications to the final evaluation.

## Gameplay

https://github.com/user-attachments/assets/a968c720-dbd3-436c-a3c1-f9890626114e

During a game cycle, the player receives a sequence of loan applications stored in a SQLite database.

For each applicant, the player can inspect several documents:

* Personal and business information
* Financial situation
* Employment situation
* Credit report
* Internal banking behaviour
* Loan details

After analysing the available information, the player must approve or reject the application.

Each decision affects the bank's budget according to the actual outcome and risk level of the loan. The objective is to evaluate every application and reach the target budget before the available time expires.

## Main Features

* Loan applications generated from a relational database
* Random selection of unique applications for each session
* Multiple draggable financial documents
* Approval and rejection system
* Comparison between player decisions and database outcomes
* Budget-based scoring system
* Configurable time limit and number of applications
* Pause, restart and result screens
* Support for Italian and English
* Resolution-independent rendering through a virtual game canvas
* Mouse-based interface designed with possible web and mobile compatibility in mind

## Technologies

| Technology         | Purpose                                             |
| ------------------ | --------------------------------------------------- |
| C++                | Core game logic                                     |
| raylib             | Rendering, input and window management              |
| SQLite             | Storage of clients, loans and financial information |
| CMake              | Project configuration                               |
| Visual Studio 2026 | Development environment                             |

## Project Architecture

The source code is divided into the game logic, banking documents, scenes,
graphical components and platform services.

```text
src/
├── main.cpp
├── gameLayer/
│   ├── documents/
│   │   ├── client.h
│   │   ├── creditReport.h
│   │   ├── document.h
│   │   ├── employmentSituation.h
│   │   ├── financialSituation.h
│   │   ├── internalBehavior.h
│   │   └── loan.h
│   ├── scenes/
│   │   ├── gameResultMenu.h
│   │   ├── initialScene.h
│   │   ├── loanExamination.h
│   │   ├── pausingMenu.h
│   │   └── startingMenu.h
│   ├── sprites/
│   │   ├── office.h
│   │   └── spriteAtlas.h
│   ├── bankDb.h
│   ├── button.h
│   ├── deltaTime.h
│   ├── documentManager.h
│   ├── game.h
│   ├── gameMain.cpp
│   ├── gameMain.h
│   ├── hud.h
│   ├── loanApplication.h
│   ├── loanApplicationManager.h
│   └── player.h
└── platform/
    ├── database.h
    ├── gameCanvas.h
    ├── imGuiHandler.h
    └── mouse.h
```

### Main Components

- `gameLayer`: contains the core gameplay logic and coordinates the game phases.
- `documents`: defines the financial and personal information associated with each loan application.
- `scenes`: contains the different game screens and their interactions.
- `sprites`: manages the office environment and sprite-sheet rendering.
- `platform`: provides lower-level services such as SQLite access, mouse input, rendering and debugging integration.
- `LoanApplicationManager`: loads and manages the current set of loan applications.
- `DocumentManager`: controls document visibility, positioning and drag-and-drop interactions.
- `BankDB`: retrieves clients, loans and financial information from the SQLite database.

### Game State

The main game flow is controlled through a finite set of phases:

```cpp
enum class GamePhase {
    InitialScene,
    StartingMenu,
    LoanExamination,
    PausingMenu,
    GameResult
};
```

Transitions between these phases are managed by the main game loop.

### Loan Application Manager

`LoanApplicationManager` coordinates the main gameplay operations:

* Selects unique loan IDs from the database
* Loads the current loan application
* Manages the application index
* Tracks the current bank budget
* Handles approval and rejection decisions
* Controls document visibility and movement
* Stores the results of evaluated applications

### Database

The SQLite database contains the information required to construct each loan application.

Main tables include:

* `clients`
* `credit_reports`
* `employment_data`
* `financial_data`
* `internal_behavior`
* `loans`

Records are connected through identifiers such as `client_id` and `loan_id`. This database-centred structure keeps the game data separate from the application logic and makes it possible to add new cases without recompiling the game.

## Decision Model

Each loan has two separate states:

* **Database status**: represents the expected or correct decision
* **Player status**: represents the decision made during gameplay

```cpp
enum class LoanStatus {
    Pending,
    Approved,
    Rejected
};
```

The separation between these states allows the game to compare the player's evaluation with the reference outcome stored in the database.

The loan model also includes information such as:

* Requested amount
* Interest rate
* Duration
* Default probability
* Recovery rate
* Guarantees
* Internal rating
* Risk level
* Remaining amount

## Controls

| Input             | Action                            |
| ----------------- | --------------------------------- |
| Left mouse button | Select buttons and drag documents |
| Approve button    | Approve the current application   |
| Reject button     | Reject the current application    |
| Pause button      | Open the pause menu               |

Development keyboard controls may also be available:

| Key | Action              |
| --- | ------------------- |
| `1` | Approve application |
| `2` | Reject application  |

## Requirements

To build the project, the following software is required:

* C++17-compatible compiler
* CMake
* Ninja or another supported CMake generator
* raylib
* SQLite

The project was primarily developed and tested on Windows 11 using Microsoft Visual C++ and Visual Studio 2026.

## Building the Project

Clone the repository:

```bash
git clone <repository-url>
cd banker-tycoon
```

Configure the project:

```bash
cmake -S . -B out/build -G Ninja
```

Build it:

```bash
cmake --build out/build
```

Run the generated executable from the build directory.

The exact executable path may depend on the selected compiler and CMake configuration.

## Educational Purpose

The game is designed to illustrate how incomplete and asymmetric information affects lending decisions.

Players must make decisions using information distributed across different documents, reproducing some of the analytical difficulties encountered by financial intermediaries when assessing borrowers.

The prototype is intended for use in an educational session with students from an Italian technical-economic secondary school.

## Project Status

The project is currently under active development.

Planned improvements include:

* Additional loan applications
* Improved scoring and feedback
* More detailed end-of-game statistics
* Enhanced document interactions
* WebAssembly build
* Touchscreen support
* Additional language support
* Expanded financial risk models

## Thesis

This software accompanies the bachelor's thesis:

**“Gamification for Understanding Information Asymmetry in Bank–Business Relationships”**

The thesis examines information asymmetry in credit markets, introduces gamification as an educational methodology, and documents the design and implementation of Banker Tycoon.

## Author

**Tommaso Pizzini**

Bachelor's Degree in Economics and Management
University of Macerata

## License

This project was developed for academic and educational purposes.

All rights reserved unless otherwise specified.
