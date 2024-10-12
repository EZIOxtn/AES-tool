Imports System.Security.Cryptography
Imports System.Text

Public Class Form1
    Private tx As String
    Private en As String
    Private pss As String
    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        tx = TextBox1.Text()
        pss = TextBox3.Text()
        If (tx.Length() = 0 Or pss.Length() = 0) Then Exit Sub
        TextBox2.Text = AES_Encrypt(tx, pss)
    End Sub
    Public Function AES_Decrypt(ByVal input As String, ByVal pass As String) As String
        Dim AES As New RijndaelManaged()
        Dim Hash_AES As New MD5CryptoServiceProvider()
        Dim decrypted As String = ""

        Try
            Dim hash(31) As Byte
            Dim temp As Byte() = Hash_AES.ComputeHash(Encoding.ASCII.GetBytes(pass))
            Array.Copy(temp, 0, hash, 0, 16)
            Array.Copy(temp, 0, hash, 15, 16)
            AES.Key = hash
            AES.Mode = CipherMode.ECB

            Dim DESDecrypter As ICryptoTransform = AES.CreateDecryptor()
            Dim Buffer As Byte() = Convert.FromBase64String(input)
            Dim decryptedBytes As Byte() = DESDecrypter.TransformFinalBlock(Buffer, 0, Buffer.Length)
            decrypted = Encoding.ASCII.GetString(decryptedBytes)

            Return decrypted
        Catch ex As Exception
         
            Return "ERROR decrypt" + ex.ToString()

        End Try
    End Function
    Public Function AES_Encrypt(ByVal input As String, ByVal pass As String) As String
        Dim AES As New RijndaelManaged()
        Dim Hash_AES As New MD5CryptoServiceProvider()
        Dim encrypted As String = ""

        Try
            Dim hash(31) As Byte
            Dim temp As Byte() = Hash_AES.ComputeHash(Encoding.ASCII.GetBytes(pass))
            Array.Copy(temp, 0, hash, 0, 16)
            Array.Copy(temp, 0, hash, 15, 16)
            AES.Key = hash
            AES.Mode = CipherMode.ECB

            Dim DESEncrypter As ICryptoTransform = AES.CreateEncryptor()
            Dim Buffer As Byte() = Encoding.ASCII.GetBytes(input)
            Dim encryptedBytes As Byte() = DESEncrypter.TransformFinalBlock(Buffer, 0, Buffer.Length)
            encrypted = Convert.ToBase64String(encryptedBytes)

            Return encrypted
        Catch ex As Exception
          
            Return "ERROR encrypt" + ex.ToString()

        End Try
    End Function

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click
        tx = TextBox1.Text()
        pss = TextBox3.Text()
        If (tx.Length() = 0 Or pss.Length() = 0) Then Exit Sub
        TextBox2.Text = AES_Decrypt(tx, pss)
    End Sub
End Class
