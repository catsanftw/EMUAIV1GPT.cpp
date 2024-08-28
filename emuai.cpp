#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QString>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <iostream>
#include <vector>
#include <cstring>

#define MEMORY_SIZE 1024

enum Instructions {
    NOP, LDI, ADD, SUB, JMP, HLT
};

struct CPU {
    int registers[4] = {0};
    int pc = 0;
    bool running = true;
};

int memory[MEMORY_SIZE];

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {}

private slots:
    void loadISO();
    void startSimulation();

private:
    QPushButton *loadButton;
    QPushButton *startButton;
    QTextEdit *textEdit;
    CPU cpu;
    
    QLabel *registersDisplay;
    QLabel *memoryDisplay;
    QStatusBar *statusBar;

    void run();
    void bios();
    void updateHUD();
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(800, 600);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    loadButton = new QPushButton("Load ISO", this);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadISO);

    startButton = new QPushButton("Start Simulation", this);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);

    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    registersDisplay = new QLabel("Registers:\nR0: 0\nR1: 0\nR2: 0\nR3: 0", this);
    memoryDisplay = new QLabel("Memory: (first 10 cells)\n[0]: 0\n[1]: 0\n[2]: 0\n...", this);

    QGroupBox *hudGroup = new QGroupBox("HUD", this);
    QVBoxLayout *hudLayout = new QVBoxLayout(hudGroup);
    hudLayout->addWidget(registersDisplay);
    hudLayout->addWidget(memoryDisplay);

    layout->addWidget(loadButton);
    layout->addWidget(startButton);
    layout->addWidget(textEdit);
    layout->addWidget(hudGroup);

    setCentralWidget(centralWidget);
    
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    bios();
}

void MainWindow::loadISO() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open ISO File", "", "ISO Files (*.iso);;All Files (*)");
    if (!fileName.isEmpty()) {
        textEdit->append("ISO Loaded: " + fileName);
        // Here you would parse the file and load it into memory
        // Simulating by loading a simple program into memory
        memory[0] = LDI; memory[1] = 1; memory[2] = 10;
        memory[3] = LDI; memory[4] = 2; memory[5] = 20;
        memory[6] = ADD; memory[7] = 3; memory[8] = 1; memory[9] = 2;
        memory[10] = HLT;
    }
}

void MainWindow::startSimulation() {
    textEdit->append("Starting the simulation...");
    cpu.running = true;
    cpu.pc = 0;
    run();
}

void MainWindow::run() {
    while (cpu.running) {
        int instruction = memory[cpu.pc++];
        switch (instruction) {
            case NOP: break;
            case LDI:
                cpu.registers[memory[cpu.pc++]] = memory[cpu.pc++];
                break;
            case ADD:
                cpu.registers[memory[cpu.pc++]] = cpu.registers[memory[cpu.pc++]] + cpu.registers[memory[cpu.pc++]];
                break;
            case SUB:
                cpu.registers[memory[cpu.pc++]] = cpu.registers[memory[cpu.pc++]] - cpu.registers[memory[cpu.pc++]];
                break;
            case JMP:
                cpu.pc = memory[cpu.pc++];
                break;
            case HLT:
                cpu.running = false;
                break;
        }
        updateHUD();
    }
    textEdit->append("Simulation finished.");
}

void MainWindow::bios() {
    // BIOS initializes memory or could load a default program
}

void MainWindow::updateHUD() {
    // Update the registers display
    QString regText = QString("Registers:\nR0: %1\nR1: %2\nR2: %3\nR3: %4")
        .arg(cpu.registers[0])
        .arg(cpu.registers[1])
        .arg(cpu.registers[2])
        .arg(cpu.registers[3]);
    registersDisplay->setText(regText);

    // Update the memory display (showing the first few cells)
    QString memText = "Memory: (first 10 cells)\n";
    for (int i = 0; i < 10; i++) {
        memText += QString("[%1]: %2\n").arg(i).arg(memory[i]);
    }
    memoryDisplay->setText(memText);

    // Update the status bar with the current PC
    statusBar->showMessage("Program Counter: " + QString::number(cpu.pc));
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "mainwindow.moc"
