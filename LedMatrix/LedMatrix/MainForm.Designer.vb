<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class MainForm
    Inherits System.Windows.Forms.Form

    'Das Formular überschreibt den Löschvorgang, um die Komponentenliste zu bereinigen.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Wird vom Windows Form-Designer benötigt.
    Private components As System.ComponentModel.IContainer

    'Hinweis: Die folgende Prozedur ist für den Windows Form-Designer erforderlich.
    'Das Bearbeiten ist mit dem Windows Form-Designer möglich.  
    'Das Bearbeiten mit dem Code-Editor ist nicht möglich.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.SerialPort = New System.IO.Ports.SerialPort(Me.components)
        Me.SerialPortComboBox = New System.Windows.Forms.ComboBox()
        Me.OpenButton = New System.Windows.Forms.Button()
        Me.SerialPortGroupBox = New System.Windows.Forms.GroupBox()
        Me.SerialPortLed = New System.Windows.Forms.PictureBox()
        Me.LedMatrixText = New System.Windows.Forms.TextBox()
        Me.LedMatrixGroupBox = New System.Windows.Forms.GroupBox()
        Me.SendButton = New System.Windows.Forms.Button()
        Me.IntensityGroupBox = New System.Windows.Forms.GroupBox()
        Me.IntensityValue = New System.Windows.Forms.TextBox()
        Me.Intensity = New System.Windows.Forms.TrackBar()
        Me.SpeedGroupBox = New System.Windows.Forms.GroupBox()
        Me.SpeedValue = New System.Windows.Forms.TextBox()
        Me.Speed = New System.Windows.Forms.TrackBar()
        Me.LogText = New System.Windows.Forms.RichTextBox()
        Me.LogGroupBox = New System.Windows.Forms.GroupBox()
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.DateiToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.BeendenToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.ÜberToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.TabControl = New System.Windows.Forms.TabControl()
        Me.TextTabPage = New System.Windows.Forms.TabPage()
        Me.LedTabPage = New System.Windows.Forms.TabPage()
        Me.LedStateMatrixGroupBox = New System.Windows.Forms.GroupBox()
        Me.SerialPortGroupBox.SuspendLayout()
        CType(Me.SerialPortLed, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.LedMatrixGroupBox.SuspendLayout()
        Me.IntensityGroupBox.SuspendLayout()
        CType(Me.Intensity, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.SpeedGroupBox.SuspendLayout()
        CType(Me.Speed, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.LogGroupBox.SuspendLayout()
        Me.MenuStrip1.SuspendLayout()
        Me.TabControl.SuspendLayout()
        Me.TextTabPage.SuspendLayout()
        Me.LedTabPage.SuspendLayout()
        Me.SuspendLayout()
        '
        'SerialPort
        '
        '
        'SerialPortComboBox
        '
        Me.SerialPortComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.SerialPortComboBox.FormattingEnabled = True
        Me.SerialPortComboBox.Location = New System.Drawing.Point(41, 43)
        Me.SerialPortComboBox.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.SerialPortComboBox.Name = "SerialPortComboBox"
        Me.SerialPortComboBox.Size = New System.Drawing.Size(160, 24)
        Me.SerialPortComboBox.TabIndex = 0
        '
        'OpenButton
        '
        Me.OpenButton.Location = New System.Drawing.Point(41, 96)
        Me.OpenButton.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.OpenButton.Name = "OpenButton"
        Me.OpenButton.Size = New System.Drawing.Size(100, 28)
        Me.OpenButton.TabIndex = 1
        Me.OpenButton.Text = "Öffnen"
        Me.OpenButton.UseVisualStyleBackColor = True
        '
        'SerialPortGroupBox
        '
        Me.SerialPortGroupBox.Controls.Add(Me.SerialPortLed)
        Me.SerialPortGroupBox.Controls.Add(Me.OpenButton)
        Me.SerialPortGroupBox.Controls.Add(Me.SerialPortComboBox)
        Me.SerialPortGroupBox.Location = New System.Drawing.Point(765, 71)
        Me.SerialPortGroupBox.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.SerialPortGroupBox.Name = "SerialPortGroupBox"
        Me.SerialPortGroupBox.Padding = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.SerialPortGroupBox.Size = New System.Drawing.Size(243, 156)
        Me.SerialPortGroupBox.TabIndex = 3
        Me.SerialPortGroupBox.TabStop = False
        Me.SerialPortGroupBox.Text = "Serieller Port"
        '
        'SerialPortLed
        '
        Me.SerialPortLed.Image = Global.LedMatrix.My.Resources.Resources.led_green_off
        Me.SerialPortLed.Location = New System.Drawing.Point(176, 99)
        Me.SerialPortLed.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.SerialPortLed.Name = "SerialPortLed"
        Me.SerialPortLed.Size = New System.Drawing.Size(25, 25)
        Me.SerialPortLed.TabIndex = 4
        Me.SerialPortLed.TabStop = False
        '
        'LedMatrixText
        '
        Me.LedMatrixText.Enabled = False
        Me.LedMatrixText.Font = New System.Drawing.Font("Microsoft Sans Serif", 14.0!)
        Me.LedMatrixText.Location = New System.Drawing.Point(67, 42)
        Me.LedMatrixText.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LedMatrixText.Name = "LedMatrixText"
        Me.LedMatrixText.Size = New System.Drawing.Size(512, 34)
        Me.LedMatrixText.TabIndex = 4
        '
        'LedMatrixGroupBox
        '
        Me.LedMatrixGroupBox.Controls.Add(Me.SendButton)
        Me.LedMatrixGroupBox.Controls.Add(Me.IntensityGroupBox)
        Me.LedMatrixGroupBox.Controls.Add(Me.SpeedGroupBox)
        Me.LedMatrixGroupBox.Controls.Add(Me.LedMatrixText)
        Me.LedMatrixGroupBox.Location = New System.Drawing.Point(31, 32)
        Me.LedMatrixGroupBox.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LedMatrixGroupBox.Name = "LedMatrixGroupBox"
        Me.LedMatrixGroupBox.Padding = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LedMatrixGroupBox.Size = New System.Drawing.Size(647, 361)
        Me.LedMatrixGroupBox.TabIndex = 5
        Me.LedMatrixGroupBox.TabStop = False
        Me.LedMatrixGroupBox.Text = "Led Matrix Text Konfiguration"
        '
        'SendButton
        '
        Me.SendButton.Enabled = False
        Me.SendButton.Location = New System.Drawing.Point(483, 300)
        Me.SendButton.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.SendButton.Name = "SendButton"
        Me.SendButton.Size = New System.Drawing.Size(100, 28)
        Me.SendButton.TabIndex = 9
        Me.SendButton.Text = "Senden"
        Me.SendButton.UseVisualStyleBackColor = True
        '
        'IntensityGroupBox
        '
        Me.IntensityGroupBox.Controls.Add(Me.IntensityValue)
        Me.IntensityGroupBox.Controls.Add(Me.Intensity)
        Me.IntensityGroupBox.Location = New System.Drawing.Point(69, 122)
        Me.IntensityGroupBox.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.IntensityGroupBox.Name = "IntensityGroupBox"
        Me.IntensityGroupBox.Padding = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.IntensityGroupBox.Size = New System.Drawing.Size(148, 207)
        Me.IntensityGroupBox.TabIndex = 7
        Me.IntensityGroupBox.TabStop = False
        Me.IntensityGroupBox.Text = "Intensität"
        '
        'IntensityValue
        '
        Me.IntensityValue.Enabled = False
        Me.IntensityValue.Location = New System.Drawing.Point(91, 106)
        Me.IntensityValue.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.IntensityValue.Name = "IntensityValue"
        Me.IntensityValue.Size = New System.Drawing.Size(41, 22)
        Me.IntensityValue.TabIndex = 10
        '
        'Intensity
        '
        Me.Intensity.Enabled = False
        Me.Intensity.Location = New System.Drawing.Point(27, 46)
        Me.Intensity.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.Intensity.Maximum = 16
        Me.Intensity.Minimum = 1
        Me.Intensity.Name = "Intensity"
        Me.Intensity.Orientation = System.Windows.Forms.Orientation.Vertical
        Me.Intensity.Size = New System.Drawing.Size(56, 140)
        Me.Intensity.TabIndex = 5
        Me.Intensity.Value = 1
        '
        'SpeedGroupBox
        '
        Me.SpeedGroupBox.Controls.Add(Me.SpeedValue)
        Me.SpeedGroupBox.Controls.Add(Me.Speed)
        Me.SpeedGroupBox.Location = New System.Drawing.Point(257, 122)
        Me.SpeedGroupBox.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.SpeedGroupBox.Name = "SpeedGroupBox"
        Me.SpeedGroupBox.Padding = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.SpeedGroupBox.Size = New System.Drawing.Size(148, 207)
        Me.SpeedGroupBox.TabIndex = 8
        Me.SpeedGroupBox.TabStop = False
        Me.SpeedGroupBox.Text = "Geschwindigkeit"
        '
        'SpeedValue
        '
        Me.SpeedValue.Enabled = False
        Me.SpeedValue.Location = New System.Drawing.Point(91, 106)
        Me.SpeedValue.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.SpeedValue.Name = "SpeedValue"
        Me.SpeedValue.Size = New System.Drawing.Size(41, 22)
        Me.SpeedValue.TabIndex = 11
        '
        'Speed
        '
        Me.Speed.Enabled = False
        Me.Speed.LargeChange = 2
        Me.Speed.Location = New System.Drawing.Point(27, 46)
        Me.Speed.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.Speed.Maximum = 100
        Me.Speed.Minimum = 1
        Me.Speed.Name = "Speed"
        Me.Speed.Orientation = System.Windows.Forms.Orientation.Vertical
        Me.Speed.Size = New System.Drawing.Size(56, 140)
        Me.Speed.TabIndex = 5
        Me.Speed.TickFrequency = 5
        Me.Speed.Value = 1
        '
        'LogText
        '
        Me.LogText.Location = New System.Drawing.Point(8, 23)
        Me.LogText.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LogText.Name = "LogText"
        Me.LogText.ReadOnly = True
        Me.LogText.Size = New System.Drawing.Size(225, 214)
        Me.LogText.TabIndex = 6
        Me.LogText.Text = ""
        '
        'LogGroupBox
        '
        Me.LogGroupBox.Controls.Add(Me.LogText)
        Me.LogGroupBox.Location = New System.Drawing.Point(765, 239)
        Me.LogGroupBox.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LogGroupBox.Name = "LogGroupBox"
        Me.LogGroupBox.Padding = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LogGroupBox.Size = New System.Drawing.Size(243, 246)
        Me.LogGroupBox.TabIndex = 5
        Me.LogGroupBox.TabStop = False
        Me.LogGroupBox.Text = "Protokoll"
        '
        'MenuStrip1
        '
        Me.MenuStrip1.ImageScalingSize = New System.Drawing.Size(20, 20)
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.DateiToolStripMenuItem, Me.ÜberToolStripMenuItem})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Padding = New System.Windows.Forms.Padding(8, 2, 0, 2)
        Me.MenuStrip1.Size = New System.Drawing.Size(1037, 28)
        Me.MenuStrip1.TabIndex = 6
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'DateiToolStripMenuItem
        '
        Me.DateiToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.BeendenToolStripMenuItem})
        Me.DateiToolStripMenuItem.Name = "DateiToolStripMenuItem"
        Me.DateiToolStripMenuItem.Size = New System.Drawing.Size(57, 24)
        Me.DateiToolStripMenuItem.Text = "Datei"
        '
        'BeendenToolStripMenuItem
        '
        Me.BeendenToolStripMenuItem.Name = "BeendenToolStripMenuItem"
        Me.BeendenToolStripMenuItem.Size = New System.Drawing.Size(142, 26)
        Me.BeendenToolStripMenuItem.Text = "Beenden"
        '
        'ÜberToolStripMenuItem
        '
        Me.ÜberToolStripMenuItem.Name = "ÜberToolStripMenuItem"
        Me.ÜberToolStripMenuItem.Size = New System.Drawing.Size(53, 24)
        Me.ÜberToolStripMenuItem.Text = "Über"
        '
        'TabControl
        '
        Me.TabControl.Controls.Add(Me.TextTabPage)
        Me.TabControl.Controls.Add(Me.LedTabPage)
        Me.TabControl.Location = New System.Drawing.Point(16, 44)
        Me.TabControl.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.TabControl.Name = "TabControl"
        Me.TabControl.SelectedIndex = 0
        Me.TabControl.Size = New System.Drawing.Size(721, 441)
        Me.TabControl.TabIndex = 7
        '
        'TextTabPage
        '
        Me.TextTabPage.BackColor = System.Drawing.SystemColors.Control
        Me.TextTabPage.Controls.Add(Me.LedMatrixGroupBox)
        Me.TextTabPage.Location = New System.Drawing.Point(4, 25)
        Me.TextTabPage.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.TextTabPage.Name = "TextTabPage"
        Me.TextTabPage.Padding = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.TextTabPage.Size = New System.Drawing.Size(713, 412)
        Me.TextTabPage.TabIndex = 0
        Me.TextTabPage.Text = "Text"
        '
        'LedTabPage
        '
        Me.LedTabPage.BackColor = System.Drawing.SystemColors.Control
        Me.LedTabPage.Controls.Add(Me.LedStateMatrixGroupBox)
        Me.LedTabPage.Location = New System.Drawing.Point(4, 25)
        Me.LedTabPage.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LedTabPage.Name = "LedTabPage"
        Me.LedTabPage.Padding = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LedTabPage.Size = New System.Drawing.Size(713, 412)
        Me.LedTabPage.TabIndex = 1
        Me.LedTabPage.Text = "LED"
        '
        'LedStateMatrixGroupBox
        '
        Me.LedStateMatrixGroupBox.Location = New System.Drawing.Point(31, 32)
        Me.LedStateMatrixGroupBox.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.LedStateMatrixGroupBox.Name = "LedStateMatrixGroupBox"
        Me.LedStateMatrixGroupBox.Padding = New System.Windows.Forms.Padding(0)
        Me.LedStateMatrixGroupBox.Size = New System.Drawing.Size(647, 361)
        Me.LedStateMatrixGroupBox.TabIndex = 0
        Me.LedStateMatrixGroupBox.TabStop = False
        Me.LedStateMatrixGroupBox.Text = "Led Matrix Zustände"
        '
        'MainForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(8.0!, 16.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1037, 501)
        Me.Controls.Add(Me.LogGroupBox)
        Me.Controls.Add(Me.SerialPortGroupBox)
        Me.Controls.Add(Me.MenuStrip1)
        Me.Controls.Add(Me.TabControl)
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog
        Me.MainMenuStrip = Me.MenuStrip1
        Me.Margin = New System.Windows.Forms.Padding(4, 4, 4, 4)
        Me.Name = "MainForm"
        Me.Text = "LedMatrix Steuerung"
        Me.SerialPortGroupBox.ResumeLayout(False)
        CType(Me.SerialPortLed, System.ComponentModel.ISupportInitialize).EndInit()
        Me.LedMatrixGroupBox.ResumeLayout(False)
        Me.LedMatrixGroupBox.PerformLayout()
        Me.IntensityGroupBox.ResumeLayout(False)
        Me.IntensityGroupBox.PerformLayout()
        CType(Me.Intensity, System.ComponentModel.ISupportInitialize).EndInit()
        Me.SpeedGroupBox.ResumeLayout(False)
        Me.SpeedGroupBox.PerformLayout()
        CType(Me.Speed, System.ComponentModel.ISupportInitialize).EndInit()
        Me.LogGroupBox.ResumeLayout(False)
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.TabControl.ResumeLayout(False)
        Me.TextTabPage.ResumeLayout(False)
        Me.LedTabPage.ResumeLayout(False)
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents SerialPort As IO.Ports.SerialPort
    Friend WithEvents SerialPortComboBox As ComboBox
    Friend WithEvents OpenButton As Button
    Friend WithEvents SerialPortGroupBox As GroupBox
    Friend WithEvents LedMatrixText As TextBox
    Friend WithEvents LedMatrixGroupBox As GroupBox
    Friend WithEvents IntensityGroupBox As GroupBox
    Friend WithEvents Intensity As TrackBar
    Friend WithEvents SpeedGroupBox As GroupBox
    Friend WithEvents Speed As TrackBar
    Friend WithEvents SendButton As Button
    Friend WithEvents LogText As RichTextBox
    Friend WithEvents SerialPortLed As PictureBox
    Friend WithEvents IntensityValue As TextBox
    Friend WithEvents SpeedValue As TextBox
    Friend WithEvents LogGroupBox As GroupBox
    Friend WithEvents MenuStrip1 As MenuStrip
    Friend WithEvents DateiToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents ÜberToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents TabControl As TabControl
    Friend WithEvents TextTabPage As TabPage
    Friend WithEvents LedTabPage As TabPage
    Friend WithEvents LedStateMatrixGroupBox As GroupBox
    Friend WithEvents BeendenToolStripMenuItem As ToolStripMenuItem
End Class
