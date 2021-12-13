import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QMainWindow, QApplication
from ui_Main import Ui_MainWindow
from RenderingType import Technique
from constants import *

ui = Ui_MainWindow()
class AppWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        
        ui.setupUi(self)

        self.dataDir = None
        
        # New Instance, Open Folder DICOM, Exit Signals
        ui.actionExit.triggered.connect(self.close)
        ui.actionNew.triggered.connect(self.new_instance)
        ui.actionOpen.triggered.connect(self.load_data)
        
        # Current Rendering Technique Changed Signal
        ui.rendering_technique.currentTextChanged.connect(self.change_rendering_technique)
        
        # ISO slider value changed, and Load Default Value
        ui.iso_value.sliderReleased.connect(lambda:update_iso(ui.iso_value.value(), ui.rendering_technique.currentText()))
        ui.iso_defaults.clicked.connect(self.load_default_iso)


        ui.skin_red.sliderReleased.connect(lambda: update_rgb(SKIN, ui.skin_red.value()/100, ui.skin_green.value()/100, ui.skin_blue.value()/100))
        ui.skin_green.sliderReleased.connect(lambda: update_rgb(SKIN, ui.skin_red.value()/100, ui.skin_green.value()/100, ui.skin_blue.value()/100))
        ui.skin_blue.sliderReleased.connect(lambda: update_rgb(SKIN, ui.skin_red.value()/100, ui.skin_green.value()/100, ui.skin_blue.value()/100))

        ui.muscle_red.sliderReleased.connect(lambda: update_rgb(MUSCLE, ui.muscle_red.value()/100, ui.muscle_green.value()/100, ui.muscle_blue.value()/100))
        ui.muscle_green.sliderReleased.connect(lambda: update_rgb(MUSCLE, ui.muscle_red.value()/100, ui.muscle_green.value()/100, ui.muscle_blue.value()/100))
        ui.muscle_blue.sliderReleased.connect(lambda: update_rgb(MUSCLE, ui.muscle_red.value()/100, ui.muscle_green.value()/100, ui.muscle_blue.value()/100))

        ui.bones_red.sliderReleased.connect(lambda: update_rgb(BONES, ui.bones_red.value()/100, ui.bones_green.value()/100, ui.bones_blue.value()/100))
        ui.bones_green.sliderReleased.connect(lambda: update_rgb(BONES, ui.bones_red.value()/100, ui.bones_green.value()/100, ui.bones_blue.value()/100))
        ui.bones_blue.sliderReleased.connect(lambda: update_rgb(BONES, ui.bones_red.value()/100, ui.bones_green.value()/100, ui.bones_blue.value()/100))
        
        ui.deep_bones_red.sliderReleased.connect(lambda: update_rgb(DEEP_BONES, ui.deep_bones_red.value()/100, ui.deep_bones_green.value()/100, ui.bones_blue.value()/100))
        ui.deep_bones_green.sliderReleased.connect(lambda: update_rgb(DEEP_BONES, ui.deep_bones_red.value()/100, ui.deep_bones_green.value()/100, ui.bones_blue.value()/100))
        ui.deep_bones_blue.sliderReleased.connect(lambda: update_rgb(DEEP_BONES, ui.deep_bones_red.value()/100, ui.deep_bones_green.value()/100, ui.bones_blue.value()/100))

        ui.skin_opacity.sliderReleased.connect(lambda: update_opacity(SKIN, ui.skin_opacity.value()/100))
        ui.muscle_opacity.sliderReleased.connect(lambda: update_opacity(MUSCLE, ui.muscle_opacity.value()/100))
        ui.bone_opacity.sliderReleased.connect(lambda: update_opacity(BONES, ui.bone_opacity.value()/100))
        ui.deep_bone_opacity.sliderReleased.connect(lambda: update_opacity(BONES, ui.bone_opacity.value()/100))

        # Load defaults of the transfer function
        ui.transfer_function_defaults.clicked.connect(self.load_default_transfer_function)
        
        self.show()

    def new_instance(self):
        self.child_window = AppWindow()
        self.child_window.show()

    
    def load_data(self):
        self.dataDir = QtWidgets.QFileDialog.getExistingDirectory(None,"Load Data", "./", QtWidgets.QFileDialog.ShowDirsOnly
                                             | QtWidgets.QFileDialog.DontResolveSymlinks )

        if self.dataDir:
            vtk_rendering(self.dataDir, ui.rendering_technique.currentText())
        
    def change_rendering_technique(self, current_rendering_technique: Technique) -> None:
        if self.dataDir:
            vtk_rendering(self.dataDir, current_rendering_technique)

    def load_default_iso(self):
        ui.iso_value.setValue(DEFAULT_ISO)
        update_iso(DEFAULT_ISO, ui.rendering_technique.currentText())

    def load_default_transfer_function(self):
        if self.dataDir:
            for node, RGB in zip(DEFAULT_REGIONS, DEFAULT_RGB):
                update_rgb(node, *RGB)
            
            for node, opacity in zip(DEFAULT_REGIONS, DEFAULT_OPACITY):
                update_opacity(node, opacity)

        ui.skin_red.setValue(DEFAULT_SKIN[RED])
        ui.skin_green.setValue(DEFAULT_SKIN[GREEN])
        ui.skin_blue.setValue(DEFAULT_SKIN[BLUE])
        ui.skin_opacity.setValue(DEFAULT_OPACITY[0])
        ui.muscle_red.setValue(DEFAULT_MUSCLE[RED])
        ui.muscle_green.setValue(DEFAULT_MUSCLE[GREEN])
        ui.muscle_blue.setValue(DEFAULT_MUSCLE[BLUE])
        ui.muscle_opacity.setValue(DEFAULT_OPACITY[1])
        ui.bones_red.setValue(DEFAULT_BONES[RED])
        ui.bones_green.setValue(DEFAULT_BONES[GREEN])
        ui.bones_blue.setValue(DEFAULT_BONES[BLUE])
        ui.bone_opacity.setValue(DEFAULT_OPACITY[2])
        ui.deep_bones_red.setValue(DEFAULT_DEEP_BONES[RED])
        ui.deep_bones_green.setValue(DEFAULT_DEEP_BONES[GREEN])
        ui.deep_bones_blue.setValue(DEFAULT_DEEP_BONES[BLUE])
        ui.deep_bone_opacity.setValue(DEFAULT_OPACITY[3])
     

app = QApplication(sys.argv)
# The class that connect Qt with VTK
from rendering import *

w = AppWindow()
# vtk_rendering()
w.show()
sys.exit(app.exec_())
# Start the event loop.
