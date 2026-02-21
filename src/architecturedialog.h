#ifndef ARCHITECTUREDIALOG_H
#define ARCHITECTUREDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <vector>


/**
 * @brief Modal dialog for editing neural network layer architecture.
 *
 * Shows fixed input/output rows (disabled) and allows adding/removing
 * hidden layers with configurable neuron count, aggregation, and activation.
 */
class ArchitectureDialog : public QDialog {
  Q_OBJECT

public:
  explicit ArchitectureDialog(const QString &currentTopology,
                              QWidget *parent = nullptr);

  /// Returns the topology string, e.g.
  /// "784_INPUT_LAYER,256_SUM_TANH,010_SUM_SMAX"
  QString topologyString() const;

private slots:
  void addLayer();
  void removeLayer(int index);
  void resetToDefault();

private:
  struct HiddenLayerRow {
    QWidget *container = nullptr;
    QSpinBox *neuronCount = nullptr;
    QComboBox *aggregation = nullptr;
    QComboBox *activation = nullptr;
    QPushButton *removeBtn = nullptr;
  };

  void rebuildLayerList();
  HiddenLayerRow createLayerRow(int neurons, int aggIdx, int actIdx);

  QVBoxLayout *layerListLayout = nullptr;
  std::vector<HiddenLayerRow> hiddenLayers;

  // Fixed layers
  static constexpr int INPUT_NEURONS = 784;
  static constexpr int OUTPUT_NEURONS = 10;

  // Default hidden layer config
  static constexpr int DEFAULT_NEURONS = 256;
  static constexpr int DEFAULT_AGG = 0; // SUM
  static constexpr int DEFAULT_ACT = 0; // TANH
};

#endif // ARCHITECTUREDIALOG_H
