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

    void run();
    void bios();
};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(600, 400);
    loadButton = new QPushButton("Load ISO", this);
    loadButton->setGeometry(50, 350, 100, 40);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadISO);

    startButton = new QPushButton("Start Simulation", this);
    startButton->setGeometry(450, 350, 150, 40);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);

    textEdit = new QTextEdit(this);
    textEdit->setGeometry(20, 20, 560, 300);
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
    }
    for (int i = 0; i < 4; ++i) {
        textEdit->append("Register " + QString::number(i) + ": " + QString::number(cpu.registers[i]));
    }
}

void MainWindow::bios() {
    // BIOS initializes memory or could load a default program
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}

#include "mainwindow.moc"
