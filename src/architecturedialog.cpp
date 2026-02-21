#include "architecturedialog.h"

#include <QFrame>
#include <QGroupBox>

// Aggregation names matching LayerType::AggregationFunction enum order
static const QStringList AGG_NAMES = {"SUM", "AVG", "MAX", "MIN"};

// Activation names matching the topology string format
static const QStringList ACT_NAMES = {
    "TANH", "RELU", "SIGMOID", "LEAKYRELU", "IDENTITY", "SOFTPLUS", "SMAX"};

ArchitectureDialog::ArchitectureDialog(const QString &currentTopology,
                                       QWidget *parent)
    : QDialog(parent) {
  setWindowTitle("Edit Architecture");
  setMinimumWidth(420);

  auto *mainLayout = new QVBoxLayout(this);

  // --- Fixed input layer (disabled) ---
  auto *inputRow = new QHBoxLayout();
  auto *inputLabel = new QLabel(QString("%1 — INPUT LAYER").arg(INPUT_NEURONS));
  inputLabel->setEnabled(false);
  inputRow->addWidget(inputLabel);
  inputRow->addStretch();
  mainLayout->addLayout(inputRow);

  // Separator
  auto *sep1 = new QFrame();
  sep1->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(sep1);

  // --- Scrollable hidden layers area ---
  auto *scrollArea = new QScrollArea();
  scrollArea->setWidgetResizable(true);
  scrollArea->setMinimumHeight(150);
  auto *scrollWidget = new QWidget();
  layerListLayout = new QVBoxLayout(scrollWidget);
  layerListLayout->setContentsMargins(0, 0, 0, 0);
  layerListLayout->addStretch(); // bottom stretch
  scrollArea->setWidget(scrollWidget);
  mainLayout->addWidget(scrollArea);

  // Separator
  auto *sep2 = new QFrame();
  sep2->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(sep2);

  // --- Fixed output layer (disabled) ---
  auto *outputRow = new QHBoxLayout();
  auto *outputLabel = new QLabel(QString("%1 — SUM SMAX").arg(OUTPUT_NEURONS));
  outputLabel->setEnabled(false);
  outputRow->addWidget(outputLabel);
  outputRow->addStretch();
  mainLayout->addLayout(outputRow);

  // Separator
  auto *sep3 = new QFrame();
  sep3->setFrameShape(QFrame::HLine);
  mainLayout->addWidget(sep3);

  // --- Buttons ---
  auto *buttonRow = new QHBoxLayout();
  auto *addBtn = new QPushButton("Add Layer");
  auto *resetBtn = new QPushButton("Reset");
  buttonRow->addWidget(addBtn);
  buttonRow->addWidget(resetBtn);
  buttonRow->addStretch();
  mainLayout->addLayout(buttonRow);

  connect(addBtn, &QPushButton::clicked, this, &ArchitectureDialog::addLayer);
  connect(resetBtn, &QPushButton::clicked, this,
          &ArchitectureDialog::resetToDefault);

  // --- OK / Cancel ---
  auto *dialogButtons =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
  connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);
  mainLayout->addWidget(dialogButtons);

  // --- Parse current topology and populate hidden layers ---
  QStringList layers = currentTopology.split(",");
  for (int i = 1; i < layers.size() - 1; ++i) { // skip input & output
    QStringList parts = layers[i].split("_");
    if (parts.size() < 3)
      continue;

    int neurons = parts[0].toInt();
    int aggIdx = AGG_NAMES.indexOf(parts[1]);
    int actIdx = ACT_NAMES.indexOf(parts[2]);
    if (aggIdx < 0)
      aggIdx = 0;
    if (actIdx < 0)
      actIdx = 0;

    hiddenLayers.push_back(createLayerRow(neurons, aggIdx, actIdx));
  }

  // If no hidden layers were parsed, add the default
  if (hiddenLayers.empty()) {
    hiddenLayers.push_back(
        createLayerRow(DEFAULT_NEURONS, DEFAULT_AGG, DEFAULT_ACT));
  }

  rebuildLayerList();
}

ArchitectureDialog::HiddenLayerRow
ArchitectureDialog::createLayerRow(int neurons, int aggIdx, int actIdx) {
  HiddenLayerRow row;
  row.container = new QWidget();
  auto *layout = new QHBoxLayout(row.container);
  layout->setContentsMargins(0, 2, 0, 2);

  row.neuronCount = new QSpinBox();
  row.neuronCount->setRange(1, 1024);
  row.neuronCount->setValue(neurons);
  row.neuronCount->setToolTip("Number of neurons in this layer");

  row.aggregation = new QComboBox();
  row.aggregation->addItems(AGG_NAMES);
  row.aggregation->setCurrentIndex(aggIdx);
  row.aggregation->setToolTip("Aggregation function");

  row.activation = new QComboBox();
  row.activation->addItems(ACT_NAMES);
  row.activation->setCurrentIndex(actIdx);
  row.activation->setToolTip("Activation function");

  row.removeBtn = new QPushButton("X");
  row.removeBtn->setFixedWidth(30);
  row.removeBtn->setToolTip("Remove this layer");

  layout->addWidget(row.neuronCount);
  layout->addWidget(row.aggregation);
  layout->addWidget(row.activation);
  layout->addWidget(row.removeBtn);

  return row;
}

void ArchitectureDialog::rebuildLayerList() {
  // Remove all items from layout except the bottom stretch
  while (layerListLayout->count() > 0) {
    auto *item = layerListLayout->takeAt(0);
    // Don't delete the widgets — we manage them in hiddenLayers
    delete item;
  }

  // Re-add rows
  for (int i = 0; i < static_cast<int>(hiddenLayers.size()); ++i) {
    layerListLayout->addWidget(hiddenLayers[i].container);
    hiddenLayers[i].container->show();

    // Reconnect remove button with correct index
    disconnect(hiddenLayers[i].removeBtn, &QPushButton::clicked, nullptr,
               nullptr);
    connect(hiddenLayers[i].removeBtn, &QPushButton::clicked, this,
            [this, i]() { removeLayer(i); });
  }

  layerListLayout->addStretch();
}

void ArchitectureDialog::addLayer() {
  hiddenLayers.push_back(
      createLayerRow(DEFAULT_NEURONS, DEFAULT_AGG, DEFAULT_ACT));
  rebuildLayerList();
}

void ArchitectureDialog::removeLayer(int index) {
  if (index < 0 || index >= static_cast<int>(hiddenLayers.size()))
    return;

  // Remove from layout and delete widget
  hiddenLayers[index].container->hide();
  hiddenLayers[index].container->deleteLater();
  hiddenLayers.erase(hiddenLayers.begin() + index);
  rebuildLayerList();
}

void ArchitectureDialog::resetToDefault() {
  // Clear all hidden layers
  for (auto &row : hiddenLayers) {
    row.container->hide();
    row.container->deleteLater();
  }
  hiddenLayers.clear();

  // Add single default layer
  hiddenLayers.push_back(
      createLayerRow(DEFAULT_NEURONS, DEFAULT_AGG, DEFAULT_ACT));
  rebuildLayerList();
}

QString ArchitectureDialog::topologyString() const {
  QString result =
      QString("%1_INPUT_LAYER").arg(INPUT_NEURONS, 3, 10, QChar('0'));

  for (const auto &row : hiddenLayers) {
    int neurons = row.neuronCount->value();
    QString agg = row.aggregation->currentText();
    QString act = row.activation->currentText();
    result +=
        QString(",%1_%2_%3").arg(neurons, 3, 10, QChar('0')).arg(agg).arg(act);
  }

  result += QString(",%1_SUM_SMAX").arg(OUTPUT_NEURONS, 3, 10, QChar('0'));

  return result;
}
