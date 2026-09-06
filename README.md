# Banker Tycoon

**Banker Tycoon** is an educational loan-assessment game developed in **C++** using **raylib** and **SQLite**.

The player acts as a bank employee and must examine loan applications, analyse the available financial documents, and decide whether each application should be accepted or rejected.

The project was created as part of a bachelor's thesis about the use of gamification to explain **information asymmetry in bank–business relationships**.

## Gameplay Demo

The following video shows a complete gameplay cycle, from the analysis of loan applications to the final evaluation of the player's decisions.

https://github.com/user-attachments/assets/a968c720-dbd3-436c-a3c1-f9890626114e

## Gameplay

During a game cycle, the player receives a sequence of loan applications stored in a SQLite database.

For each applicant, the player can inspect several documents:

* Personal and business information
* Financial situation
* Employment situation
* Credit report
* Internal banking behaviour
* Loan details

After analysing the available information, the player must accept or reject the application.

Each decision affects the bank's budget according to the actual outcome and risk level of the loan. The objective is to evaluate every application and reach the target budget before the available time expires.

At the end of the game, the player can review the individual applications, compare the submitted decisions with the expected outcomes and inspect their financial results.

## Main Features

* Loan applications generated from a relational database
* Random selection of unique applications for each game session
* Multiple draggable financial documents
* Acceptance and rejection system
* Comparison between player decisions and database outcomes
* Budget-based scoring system
* Configurable time limit and number of applications
* Individual loan-result review
* Pause, restart and result screens
* Resolution-independent rendering through a virtual game canvas
* Mouse-based interface designed with future web and touchscreen compatibility in mind

## Technologies

| Technology         | Purpose                                             |
| ------------------ | --------------------------------------------------- |
| C++                | Core game and domain logic                          |
| raylib             | Rendering, input and window management              |
| SQLite             | Storage of clients, loans and financial information |
| CMake              | Project configuration and build management          |
| Visual Studio 2026 | Development environment and MSVC toolchain          |

## Project Architecture

The source code is divided into game logic, banking documents, scenes, graphical components and platform services.

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

* `Game` stores the current phase, timer, configuration and player.
* `LoanApplicationManager` coordinates loan selection, decisions, budget changes and results.
* `LoanApplication` aggregates all the information associated with one loan request.
* `DocumentManager` controls document visibility, order, positioning and drag-and-drop interactions.
* `BankDB` retrieves and updates clients, loans and financial information in the SQLite database.
* `GameCanvas` provides resolution-independent rendering through a virtual canvas.
* `scenes` contains the different game screens and their interactions.
* `sprites` manages the office environment and sprite-sheet rendering.
* `platform` contains lower-level services for database access, mouse input and rendering support.

## Game State

The main game flow is represented through a finite set of phases:

```cpp
enum class GamePhase {
    InitialScene,
    StartingMenu,
    LoanExamination,
    PausingMenu,
    GameResult
};
```

Transitions between these phases are coordinated by the main game loop.

The principal flow is:

```text
Initial scene
    ↓
Starting menu
    ↓
Loan examination
    ↓
Pause menu or next application
    ↓
Game results
```

## Domain Composition

A `Game` contains a `Player` and manages the current game state.

A `LoanApplicationManager` contains the active `LoanApplication` and manages the sequence of applications selected for the current session.

Each `LoanApplication` is composed of:

* `Loan`
* `Client`
* `CreditReport`
* `EmploymentSituation`
* `FinancialSituation`
* `InternalBehavior`

This composition keeps the data belonging to one application together while allowing each document type to have its own model and responsibilities.

The `LoanExamination` scene contains:

* An `OfficeSprite`
* A `DocumentManager`
* A `Hud`
* Buttons for accepting, rejecting and advancing to the next application

The `GameResultMenu` uses a separate `DocumentManager` to display the outcome of each evaluated loan.

## Loan Application Manager

`LoanApplicationManager` coordinates the main gameplay operations:

* Selects unique loan IDs from the database
* Loads the current loan application
* Manages the application index
* Tracks the current bank budget
* Handles acceptance and rejection decisions
* Updates loan statuses in the database
* Calculates the financial result of each decision
* Stores the evaluated applications for the final review

## Document System

`DocumentManager` maintains a collection of financial documents associated with the current application.

It is responsible for:

* Creating document buttons
* Opening and closing documents
* Tracking document visibility
* Managing document drawing order
* Bringing selected documents to the foreground
* Handling drag-and-drop movement
* Displaying the result document during the final review

The individual `Document` objects determine which type of banking information must be displayed.

## Database

The SQLite database contains the information required to construct each loan application.

The main tables include:

* `clients`
* `credit_reports`
* `employment_data`
* `financial_data`
* `internal_behavior`
* `loans`

Records are connected through identifiers such as `client_id` and `loan_id`.

This database-centred structure separates persistent data from the game logic and makes it possible to add or modify loan cases without recompiling the application.

`BankDB` provides the interface between the game and SQLite. It retrieves the records required to construct a `LoanApplication` and updates loan statuses during gameplay.

## Decision Model

Each loan has two separate states:

* **Database status**: represents the expected reference decision
* **Player status**: represents the decision made during gameplay

```cpp
enum class LoanStatus {
    Pending,
    Approved,
    Rejected
};
```

The separation between these states allows the game to compare the player's evaluation with the reference outcome stored in the database.

The loan model also includes:

* Requested amount
* Interest rate
* Duration
* Default probability
* Recovery rate
* Guarantees
* Internal rating
* Risk level
* Remaining amount
* Decision reason

## Controls

| Input             | Action                              |
| ----------------- | ----------------------------------- |
| Left mouse button | Select buttons and drag documents   |
| Accept button     | Accept the current loan application |
| Reject button     | Reject the current loan application |
| Next Loan button  | Continue to the next application    |
| Pause button      | Open the pause menu                 |

## Requirements

The project was primarily developed and tested on **Windows 11** using **Visual Studio 2026** and the Microsoft Visual C++ toolchain.

The following software is required:

* Visual Studio 2026
* Desktop development with C++ workload
* CMake
* Git

The source code for raylib and SQLite is included in the project's `thirdparty` directory and configured through CMake.

## Educational Purpose

The game is designed to illustrate how incomplete and asymmetric information affects lending decisions.

Players must make decisions using information distributed across several documents, reproducing some of the analytical difficulties encountered by financial intermediaries when assessing borrowers.

The prototype is intended for use during an educational session with students from an Italian technical-economic secondary school.

The game does not attempt to reproduce a complete professional credit-scoring system. Its purpose is to provide a simplified and interactive representation of the relationship between information, risk and lending decisions.

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
* Expanded financial-risk models

> A public playable build is not currently available. The repository contains
> the source code and development build instructions.

## Thesis

This software accompanies the bachelor's thesis:

**“Gamification for Understanding Information Asymmetry in Bank–Business Relationships”**

The thesis examines information asymmetry in credit markets, introduces gamification as an educational methodology, and documents the design and implementation of Banker Tycoon.

## Author

**Tommaso Pizzini**
Bachelor's Degree in Economics and Management
University of Macerata

## Credits and Acknowledgements

### Visual Assets

All sprites used in Banker Tycoon, except for the document graphics, come from
[Pixel Life: Office Essentials](https://christianperich.itch.io/pixel-life-office-essentials)
by **Chris Perich**.

The asset pack is distributed under the
[Creative Commons Attribution 4.0 International licence](https://creativecommons.org/licenses/by/4.0/).

The sprites were integrated and, where necessary, adapted for use within
Banker Tycoon. The document graphics were created specifically for this project.

### C++ and CMake Foundation

The initial C++ project structure and CMake configuration were based on
materials provided in a Udemy course created by
[meemknight (Vlod)](https://github.com/meemknight).

The original course setup was subsequently adapted and extended for Banker
Tycoon, including the integration of raylib, SQLite, the loan-assessment domain
model, the document-management system and the game-specific architecture.

## License

This project was developed for academic and educational purposes.

All rights reserved unless otherwise specified.
