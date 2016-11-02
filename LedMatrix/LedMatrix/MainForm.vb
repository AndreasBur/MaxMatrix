Class MainForm

#Region "Definitionen"
    Private Class LedArrayTag
        Public Column As Integer
        Public Row As Integer
        Public LedOnState As Boolean
    End Class
#End Region

#Region "Function Prototypes"
    Delegate Sub SetTextCallback(ByVal Text As String)
#End Region

#Region "Global Variables"
    Dim PortOpenState As Boolean 'port state opened/closed
    Dim LedArray(8, 8) As PictureBox
#End Region

#Region "Form"
    Private Sub MainForm_Load(sender As Object, e As EventArgs) Handles MyBase.Load
        PortOpenState = False
        IntensityValue.Text = Intensity.Value.ToString
        SpeedValue.Text = Speed.Value.ToString
        SerialComs()

        For n = 0 To 7 Step 1
            For i = 0 To 7 Step 1
                LedArray(n, i) = New PictureBox
                LedArray(n, i).Image = LedMatrix.My.Resources.led_green_off
                'edArray(n, i).Enabled = False
                LedArray(n, i).Location = New Point(125 + i * 30, 35 + n * 30)
                LedArray(n, i).Size = New Size(25, 25)
                LedStateMatrixGroupBox.Controls.Add(LedArray(n, i))
                AddHandler LedArray(n, i).Click, AddressOf LedArray_Click
                LedArray(n, i).Tag = New LedArrayTag
                LedArray(n, i).Tag.Column = n
                LedArray(n, i).Tag.Row = i
                LedArray(n, i).Tag.LedOnState = False
            Next
        Next
    End Sub

    Private Sub MainForm_FormClosed(sender As Object, e As FormClosedEventArgs) Handles MyBase.FormClosed
        SerialPort.Close()
    End Sub
#End Region


#Region "Subs"
    Sub SerialComs()
        'Zeigt alle verfügbaren COM-Ports in ComboBox1
        For Each COMPort As String In My.Computer.Ports.SerialPortNames
            SerialPortComboBox.Items.Add(COMPort)
        Next
    End Sub

    Private Sub SerialPort1_DataReceived(sender As System.Object, e As System.IO.Ports.SerialDataReceivedEventArgs) Handles SerialPort.DataReceived
        'Liest Daten auf SerialPort1
        ReceivedText(SerialPort.ReadExisting())
    End Sub

    Private Sub ReceivedText(ByVal Text As String)
        'Leitet empfangene Daten mittels des Delegaten von SerialPort1 in RTB1
        If LogText.InvokeRequired Then
            Dim x As New SetTextCallback(AddressOf ReceivedText)
            Invoke(x, New Object() {(Text)})
        Else
            Text = Text.Replace(vbCrLf, "").Replace(vbCr, "")
            LogText.AppendText(Text + vbCrLf)
        End If
    End Sub
#End Region


#Region "control elements"
    Private Sub OpenButton_Click(sender As Object, e As EventArgs) Handles OpenButton.Click
        If PortOpenState = False Then
            If SerialPortComboBox.SelectedIndex <> -1 Then
                SerialPort.PortName = SerialPortComboBox.SelectedItem
                SerialPort.Open()
                SerialPortLed.Image = LedMatrix.My.Resources.led_green_on
                OpenButton.Text = "Schließen"
                SendButton.Enabled = True
                Speed.Enabled = True
                Intensity.Enabled = True
                LedMatrixText.Enabled = True
                IntensityValue.Enabled = True
                SpeedValue.Enabled = True
                PortOpenState = True
            End If
        Else
            SerialPort.Close()
            SerialPortLed.Image = LedMatrix.My.Resources.led_green_off
            OpenButton.Text = "Öffnen"
            SendButton.Enabled = False
            Speed.Enabled = False
            Intensity.Enabled = False
            LedMatrixText.Enabled = False
            IntensityValue.Enabled = False
            SpeedValue.Enabled = False
            PortOpenState = False
        End If
    End Sub

    Private Sub SendButton_Click(sender As Object, e As EventArgs) Handles SendButton.Click
        SerialPort.WriteLine("text=" + LedMatrixText.Text)
    End Sub

    Private Sub Intensity_Scroll(sender As Object, e As EventArgs) Handles Intensity.Scroll
        SerialPort.WriteLine("intensity=" + Intensity.Value.ToString)
        IntensityValue.Text = Intensity.Value.ToString
        'LedMatrixText.Text = Intensity.Value.ToString
    End Sub

    Private Sub Speed_Scroll(sender As Object, e As EventArgs) Handles Speed.Scroll
        SerialPort.WriteLine("speed=" + Speed.Value.ToString)
        SpeedValue.Text = Speed.Value.ToString
        'LedMatrixText.Text = Speed.Value.ToString
    End Sub

    Private Sub LedArray_Click(sender As Object, e As EventArgs)
        Dim Led = DirectCast(sender, PictureBox)

        If Led.Tag.LedOnState = False Then
            Led.Tag.LedOnState = True
            Led.Image = LedMatrix.My.Resources.led_green_on
        ElseIf Led.Tag.LedOnState = True Then
            Led.Tag.LedOnState = False
            Led.Image = LedMatrix.My.Resources.led_green_off
        End If

        If PortOpenState = True Then
            SerialPort.WriteLine("SetDot=" + Led.Tag.Column.ToString + "," + Led.Tag.Row.ToString + "," + Led.Tag.LedOnState.ToString)
        End If
        MsgBox("SetDot=" + Led.Tag.Column.ToString + "," + Led.Tag.Row.ToString + "," + Led.Tag.LedOnState.ToString)

    End Sub
#End Region

End Class
