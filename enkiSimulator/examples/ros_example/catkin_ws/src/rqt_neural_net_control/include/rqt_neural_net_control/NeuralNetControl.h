#pragma once

#include <rqt_gui_cpp/plugin.h>
#include <rqt_neural_net_control/ui_NeuralNetControl.h>

#include <ros/ros.h>
#include "rqt_neural_net_control/GetNeuralNetConfig.h" //srv definition

#include <QWidget>
#include <QObject>

#include <QHBoxLayout>  //for the layout of each layer
#include <QLabel>       //for the layer number
#include <QComboBox>    //for the layer activation options
#include <QSpinBox>     //for the neuron counts

namespace neuralnetcontrol {

class NeuralNetControl : public rqt_gui_cpp::Plugin {
Q_OBJECT

public:

  /* ======================================================================== */
  /* Constructor/Destructor                                                   */
  /* ======================================================================== */

  NeuralNetControl();

  /* ======================================================================== */
  /* Initialize/Shutdown                                                      */
  /* ======================================================================== */

  void initPlugin(qt_gui_cpp::PluginContext& context) override;

  void shutdownPlugin() override;

  /* ======================================================================== */
  /* Settings                                                                 */
  /* ======================================================================== */

  void saveSettings(qt_gui_cpp::Settings& plugin_settings,
                    qt_gui_cpp::Settings& instance_settings) const override;

  void restoreSettings(const qt_gui_cpp::Settings& plugin_settings,
                       const qt_gui_cpp::Settings& instance_settings) override;

private:

  /* ======================================================================== */
  /* Constants                                                                */
  /* ======================================================================== */

  const std::string TAG = "NeuralNetControl";
  
  const QStringList activationFunctions = {"tanh","asinh","sigmoid","ReLU","Leaky ReLU","softmax", "linear"};
  const QStringList convolutionOptions = {"yes", "no"};
  const QStringList optimiserFunctions = { "sgd", "momentum", "adagrad", "rmsprop", "adam", "adamax", "gradient decent"};
  const QStringList lossFunctions = { "cross entropy", "mse" };

  /* ======================================================================== */
  /* Variables                                                                */
  /* ======================================================================== */

  Ui::NeuralNetControlWidget ui_;
  QWidget* widget_;

  ros::NodeHandle nh_;
  ros::ServiceServer get_config_service;

  std::vector<QHBoxLayout*> layerEntries;
  std::vector<QComboBox*> layerEntriesComboBox;
  std::vector<QSpinBox*> layerEntriesSpinBox;

  /* ======================================================================== */
  /* Methods                                                                  */
  /* ======================================================================== */



  /* ======================================================================== */
  /* Events                                                                   */
  /* ======================================================================== */



  /* ======================================================================== */
  /* Callbacks                                                                */
  /* ======================================================================== */

  bool GetNeuralNetworkConfig_callback(rqt_neural_net_control::GetNeuralNetConfig::Request &req,
                                       rqt_neural_net_control::GetNeuralNetConfig::Response &res);


protected slots:

  /* ======================================================================== */
  /* Slots                                                                    */
  /* ======================================================================== */

  void addLayer();
  void deleteLayer();

signals:

  /* ======================================================================== */
  /* Signals                                                                  */
  /* ======================================================================== */



};

} // namespace
