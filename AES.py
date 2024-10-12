from Crypto.Cipher import AES
from Crypto.Hash import MD5
import base64

def create_key(password):
    hash_obj = MD5.new()
    hash_obj.update(password.encode('utf-8'))
    return hash_obj.digest()

def aes_encrypt(input_text, password):
    try:
        key = create_key(password)
        cipher = AES.new(key, AES.MODE_ECB)
        padded_input = input_text + (AES.block_size - len(input_text) % AES.block_size) * chr(AES.block_size - len(input_text) % AES.block_size)
        encrypted = cipher.encrypt(padded_input.encode('utf-8'))
        return base64.b64encode(encrypted).decode('utf-8')
    except Exception as e:
        return f"ERROR encrypt: {e}"

def aes_decrypt(input_text, password):
    try:
        key = create_key(password)
        cipher = AES.new(key, AES.MODE_ECB)
        decrypted = cipher.decrypt(base64.b64decode(input_text))
        padding_length = decrypted[-1]
        return decrypted[:-padding_length].decode('utf-8')
    except Exception as e:
        return f"ERROR decrypt: {e}"

if __name__ == "__main__":
    text = input("Enter text to encrypt: ")
    password = input("Enter password: ")

    if not text or not password:
        print("Text or password cannot be empty.")
    else:
        encrypted_text = aes_encrypt(text, password)
        print(f"Encrypted text: {encrypted_text}")
        decrypted_text = aes_decrypt(encrypted_text, password)
        print(f"Decrypted text: {decrypted_text}")
