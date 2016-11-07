Imports System.IO.Ports
Imports System.Windows.Threading


Class MainWindow

#Region "Definitionen"
    Private Class LedArrayTag
        Public LedModule As Integer
        Public Column As Integer
        Public Row As Integer
        Public LedOnState As Boolean
    End Class

    Public Const LedMatrixNumberOfColumns = 8
    Public Const LedMatrixNumberOfRows = 8
#End Region

#Region "Function Prototypes"
    Delegate Sub SetTextCallback(ByVal Text As String)
#End Region

#Region "Global Variables"
    Dim PortOpenState As Boolean 'port state opened/closed
    Dim Serial As SerialPort = New SerialPort
    Dim LedArray(,,) As Image
    Private this As Window
#End Region

#Region "Subs"
    Sub SerialComs()
        SerialPortComboBox.Items.Clear()
        'Zeigt alle verfügbaren COM-Ports in ComboBox
        For Each COMPort As String In My.Computer.Ports.SerialPortNames
            SerialPortComboBox.Items.Add(COMPort)
        Next
    End Sub

    Private Sub Serial_DataReceivedHandler(sender As System.Object, e As System.IO.Ports.SerialDataReceivedEventArgs)
        'Liest Daten auf SerialPort
        ReceivedText(Serial.ReadExisting())
    End Sub

    Private Sub ReceivedText(ByVal Text As String)
        If LogTextBox.Dispatcher.CheckAccess() Then
            LogTextBox.AppendText(Text)
        Else
            LogTextBox.Dispatcher.Invoke(DispatcherPriority.Normal, New SetTextCallback(AddressOf ReceivedText), Text)
        End If
    End Sub

    Private Sub EnableControls(ByVal EnableState As Boolean)
        LedMatrixText.IsEnabled = EnableState
        IntensitySlider.IsEnabled = EnableState
        'SpeedSlider.IsEnabled = EnableState
        IntensityValue.IsEnabled = EnableState
        'SpeedValue.IsEnabled = EnableState
        DeleteButton.IsEnabled = EnableState
        SendButton.IsEnabled = EnableState
        ShiftComboBox.IsEnabled = EnableState
        LedMatrixModuleComboBox.IsEnabled = EnableState
        SpaceComboBox.IsEnabled = EnableState

        For LedModule = 0 To LedMatrixModuleComboBox.Items.Count - 1 Step 1
            For Column = 0 To LedMatrixNumberOfColumns - 1 Step 1
                For Row = 0 To LedMatrixNumberOfRows - 1 Step 1
                    LedArray(LedModule, Column, Row).IsEnabled = EnableState
                Next
            Next
        Next
    End Sub

    Private Sub PrintLedMatrix(ByVal LedModule As Integer)
        LedMatrixStateGrid.Children.Clear()
        For Column = 0 To LedMatrixNumberOfColumns - 1 Step 1
            For Row = 0 To LedMatrixNumberOfRows - 1 Step 1
                LedMatrixStateGrid.Children.Add(LedArray(LedMatrixModuleComboBox.SelectedIndex, Column, Row))
            Next
        Next
    End Sub
#End Region

#Region "Control Elements"

    Private Sub Window_Loaded(sender As Object, e As RoutedEventArgs)
        PortOpenState = False
        Serial.Encoding = System.Text.Encoding.Default
        SerialComs()
        AddHandler Serial.DataReceived, AddressOf Serial_DataReceivedHandler

        LedArray = New Image(LedMatrixModuleComboBox.Items.Count, LedMatrixNumberOfColumns - 1, LedMatrixNumberOfRows - 1) {}

        For LedModule = 0 To LedMatrixModuleComboBox.Items.Count - 1 Step 1
            For Row = 0 To LedMatrixNumberOfRows - 1 Step 1
                For Column = 0 To LedMatrixNumberOfColumns - 1 Step 1
                    LedArray(LedModule, Column, Row) = New Image
                    LedArray(LedModule, Column, Row).Source = New BitmapImage(New Uri("pack://siteoforigin:,,,/Resources/led_green_off_big.png", UriKind.RelativeOrAbsolute))
                    LedArray(LedModule, Column, Row).IsEnabled = False
                    LedArray(LedModule, Column, Row).Width = 25
                    LedArray(LedModule, Column, Row).Height = 25
                    LedArray(LedModule, Column, Row).VerticalAlignment = VerticalAlignment.Top
                    LedArray(LedModule, Column, Row).HorizontalAlignment = HorizontalAlignment.Left
                    LedArray(LedModule, Column, Row).Margin = New Thickness(20 + Column * 30, 25 + Row * 30, 0, 0)
                    LedArray(LedModule, Column, Row).AddHandler(Image.MouseLeftButtonUpEvent, New RoutedEventHandler(AddressOf LedArray_Click))
                    LedArray(LedModule, Column, Row).Tag = New LedArrayTag
                    LedArray(LedModule, Column, Row).Tag.LedModule = LedModule
                    LedArray(LedModule, Column, Row).Tag.Column = Column
                    LedArray(LedModule, Column, Row).Tag.Row = Row
                    LedArray(LedModule, Column, Row).Tag.LedOnState = False
                Next
            Next
        Next

        PrintLedMatrix(LedMatrixModuleComboBox.SelectedIndex)
    End Sub

    Private Sub Window_Closed(sender As Object, e As EventArgs)
        Serial.Close()
    End Sub

    Private Sub LedArray_Click(sender As Object, e As EventArgs)
        Dim Led = DirectCast(sender, Image)

        If Led.Tag.LedOnState = False Then
            Led.Tag.LedOnState = True
            Led.Source = New BitmapImage(New Uri("pack://siteoforigin:,,,/Resources/led_green_on_big.png", UriKind.RelativeOrAbsolute))
        ElseIf Led.Tag.LedOnState = True Then
            Led.Tag.LedOnState = False
            Led.Source = New BitmapImage(New Uri("pack://siteoforigin:,,,/Resources/led_green_off_big.png", UriKind.RelativeOrAbsolute))
        End If

        If PortOpenState = True Then
            Dim Column As Integer
            Column = (Led.Tag.LedModule * LedMatrixNumberOfColumns) + Led.Tag.Column.ToString
            'MsgBox("setDot=" + Led.Tag.LedModule.ToString + "," + Led.Tag.Column.ToString + "," + Led.Tag.Row.ToString)
            If Led.Tag.LedOnState Then
                Serial.WriteLine("setDot=" + Column.ToString + "," + Led.Tag.Row.ToString + "," + "1")
            Else
                Serial.WriteLine("setDot=" + Column.ToString + "," + Led.Tag.Row.ToString + "," + "0")
            End If
        End If
    End Sub

    Private Sub SendButton_Click(sender As Object, e As RoutedEventArgs) Handles SendButton.Click
        If ShiftComboBox.SelectedItem.Content.ToString() = "Ein" Then
            Serial.WriteLine("textShift=" + LedMatrixText.Text)
        Else
            Serial.WriteLine("text=" + LedMatrixText.Text)
        End If
    End Sub

    Private Sub OpenButton_Click(sender As Object, e As RoutedEventArgs) Handles OpenButton.Click
        If PortOpenState = False Then
            If SerialPortComboBox.SelectedIndex <> -1 Then
                Serial.PortName = SerialPortComboBox.SelectedItem
                Serial.Open()
                SerialPortStatusImage.Source = New BitmapImage(New Uri("pack://siteoforigin:,,,/Resources/led_green_on_big.png", UriKind.RelativeOrAbsolute))
                OpenButton.Content = "Schließen"
                EnableControls(True)
                PortOpenState = True
            End If
        Else
            Serial.Close()
            SerialPortStatusImage.Source = New BitmapImage(New Uri("pack://siteoforigin:,,,/Resources/led_green_off_big.png", UriKind.RelativeOrAbsolute))
            OpenButton.Content = "Öffnen"
            EnableControls(False)
            PortOpenState = False
        End If
    End Sub

    Private Sub DeleteButton_Click(sender As Object, e As RoutedEventArgs) Handles DeleteButton.Click
        LedMatrixText.Clear()
        LogTextBox.Clear()
    End Sub

    Private Sub IntensitySlider_ValueChanged(sender As Object, e As RoutedPropertyChangedEventArgs(Of Double)) Handles IntensitySlider.ValueChanged
        If PortOpenState = True Then
            Serial.WriteLine("intensity=" + IntensitySlider.Value.ToString)
        End If
    End Sub

    Private Sub SpeedSlider_ValueChanged(sender As Object, e As RoutedPropertyChangedEventArgs(Of Double)) Handles SpeedSlider.ValueChanged
        If PortOpenState = True Then
            Serial.WriteLine("speed=" + SpeedSlider.Value.ToString)
        End If
    End Sub

    Private Sub CloseMenuItem_Click(sender As Object, e As RoutedEventArgs)
        System.Windows.Application.Current.MainWindow.Close()
    End Sub

    Private Sub AboutMenuItem_Click(sender As Object, e As RoutedEventArgs)
        Dim About As AboutWindow = New AboutWindow
        About.Owner = Application.Current.MainWindow
        About.WindowStartupLocation = WindowStartupLocation.CenterOwner
        About.Show()
    End Sub

    Private Sub SerialPortComboBox_DropDownOpened(sender As Object, e As EventArgs) Handles SerialPortComboBox.DropDownOpened
        SerialComs()
    End Sub

    Private Sub LedMatrixModuleComboBox_DropDownClosed(sender As Object, e As EventArgs) Handles LedMatrixModuleComboBox.DropDownClosed
        PrintLedMatrix(LedMatrixModuleComboBox.SelectedIndex)
    End Sub

    Private Sub Shift_SelectionChanged(sender As Object, e As SelectionChangedEventArgs) Handles ShiftComboBox.SelectionChanged
        If ShiftComboBox.SelectedItem.Content.ToString = "Ein" And PortOpenState = True Then
            SpeedSlider.IsEnabled = True
            SpeedValue.IsEnabled = True
        Else
            SpeedSlider.IsEnabled = False
            SpeedValue.IsEnabled = False
        End If
    End Sub

    Private Sub Space_SelectionChanged(sender As Object, e As SelectionChangedEventArgs) Handles SpaceComboBox.SelectionChanged
        If PortOpenState = True Then
            Serial.WriteLine("space=" + SpaceComboBox.SelectedItem.Content.ToString)
        End If
    End Sub
#End Region

End Class
