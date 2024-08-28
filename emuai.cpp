#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QString>
#include <iostream>
#include <vector>
#include <cstring>

#define MEMORY_SIZE 1024
#define STACK_SIZE 256

enum Instructions {
    NOP, LDI, ADD, SUB, MUL, DIV, MOD, AND, OR, XOR, SHL, SHR, JMP, HLT, PUSH, POP, CALL, RET
};

struct CPU {
    int registers[4] = {0};
    int pc = 0;
    int sp = MEMORY_SIZE; // Stack pointer starts at the end of memory
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

    void run();
    void bios();
    void executeInstruction(int instruction);
    void push(int value);
    int pop();
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(800, 600);
    loadButton = new QPushButton("Load ISO", this);
    loadButton->setGeometry(50, 500, 100, 40);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadISO);

    startButton = new QPushButton("Start Simulation", this);
    startButton->setGeometry(650, 500, 150, 40);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);

    textEdit = new QTextEdit(this);
    textEdit->setGeometry(20, 20, 760, 450);
    textEdit->setReadOnly(true);

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
        memory[10] = MUL; memory[11] = 3; memory[12] = 1; memory[13] = 2;
        memory[14] = HLT;
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
        executeInstruction(instruction);
    }
    for (int i = 0; i < 4; ++i) {
        textEdit->append("Register " + QString::number(i) + ": " + QString::number(cpu.registers[i]));
    }
}

void MainWindow::bios() {
    // Initialize stack pointer and load bootloader or default program
    cpu.sp = MEMORY_SIZE - 1; // Stack grows downwards
    // Additional BIOS setup
}

void MainWindow::executeInstruction(int instruction) {
    switch (instruction) {
        case NOP: break;
        case LDI:
            cpu.registers[memory[cpu.pc++]] = memory[cpu.pc++];
            break;
        case ADD:
            cpu.registers[memory[cpu.pc++]] = cpu.registers[memory[cpu.pc++]] + cpu.registers[memory[cpu.pc++]];
            break;
        case MUL:
            cpu.registers[memory[cpu.pc++]] = cpu.registers[memory[cpu.pc++]] * cpu.registers[memory[cpu.pc++]];
            break;
        case HLT:
            cpu.running = false;
            break;
        // Other cases for new instructions
        default:
            textEdit->append("Unknown instruction at PC: " + QString::number(cpu.pc));
            cpu.running = false;
            break;
    }
}

void MainWindow::push(int value) {
    if (cpu.sp <= 0) {
        textEdit->append("Stack overflow!");
        cpu.running = false;
        return;
    }
    memory[--cpu.sp] = value;
}

int MainWindow::pop() {
    if (cpu.sp >= MEMORY_SIZE) {
        textEdit->append("Stack underflow!");
        cpu.running = false;
        return -1;
    }
    return memory[cpu.sp++];
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "mainwindow.moc"
