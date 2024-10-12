require 'openssl'
require 'base64'

def aes_encrypt(input, pass)
  aes = OpenSSL::Cipher.new('AES-128-ECB')
  hash = OpenSSL::Digest::MD5.digest(pass)

  aes.encrypt
  aes.key = hash
  encrypted = aes.update(input) + aes.final
  Base64.encode64(encrypted)
rescue => e
  "ERROR encrypt: #{e.message}"
end

def aes_decrypt(input, pass)
  aes = OpenSSL::Cipher.new('AES-128-ECB')
  hash = OpenSSL::Digest::MD5.digest(pass)

  aes.decrypt
  aes.key = hash
  decrypted = aes.update(Base64.decode64(input)) + aes.final
  decrypted
rescue => e
  "ERROR decrypt: #{e.message}"
end


puts 'Enter text to encrypt:'
text = gets.chomp
puts 'Enter password:'
password = gets.chomp

if text.empty? || password.empty?
  puts 'Text or password cannot be empty.'
else
  encrypted_text = aes_encrypt(text, password)
  puts "Encrypted text: #{encrypted_text}"

  decrypted_text = aes_decrypt(encrypted_text, password)
  puts "Decrypted text: #{decrypted_text}"
end
