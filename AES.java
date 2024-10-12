import javax.crypto.Cipher;
import javax.crypto.spec.SecretKeySpec;
import javax.xml.bind.DatatypeConverter;
import java.security.MessageDigest;
import java.util.Scanner;

public class AESCrypt {
    public static String aesEncrypt(String input, String pass) {
        try {
            byte[] key = createKey(pass);
            Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
            cipher.init(Cipher.ENCRYPT_MODE, new SecretKeySpec(key, "AES"));
            byte[] encryptedBytes = cipher.doFinal(input.getBytes("UTF-8"));
            return DatatypeConverter.printBase64Binary(encryptedBytes);
        } catch (Exception e) {
            return "ERROR encrypt: " + e.getMessage();
        }
    }

    public static String aesDecrypt(String input, String pass) {
        try {
            byte[] key = createKey(pass);
            Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
            cipher.init(Cipher.DECRYPT_MODE, new SecretKeySpec(key, "AES"));
            byte[] decryptedBytes = cipher.doFinal(DatatypeConverter.parseBase64Binary(input));
            return new String(decryptedBytes, "UTF-8");
        } catch (Exception e) {
            return "ERROR decrypt: " + e.getMessage();
        }
    }

    private static byte[] createKey(String pass) throws Exception {
        MessageDigest digest = MessageDigest.getInstance("MD5");
        byte[] key = new byte[16];
        System.arraycopy(digest.digest(pass.getBytes("UTF-8")), 0, key, 0, 16);
        return key;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter text to encrypt: ");
        String text = scanner.nextLine();
        System.out.print("Enter password: ");
        String password = scanner.nextLine();

        if (text.isEmpty() || password.isEmpty()) {
            System.out.println("Text or password cannot be empty.");
        } else {
            String encryptedText = aesEncrypt(text, password);
            System.out.println("Encrypted text: " + encryptedText);
            String decryptedText = aesDecrypt(encryptedText, password);
            System.out.println("Decrypted text: " + decryptedText);
        }
    }
}
