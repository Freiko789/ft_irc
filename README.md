# ft_irc

**ft_irc** est une application de serveur IRC (Internet Relay Chat) en C++ développée dans le cadre de l'école 42.
Ce projet simule un serveur IRC où les utilisateurs peuvent se connecter, s'inscrire à des canaux,
envoyer des messages privés et publics, et gérer des discussions en temps réel.

## 🚀 Technologies utilisées

- **C++** : Langage principal pour le développement du projet.
- **Sockets** : Utilisation des sockets pour la communication réseau en temps réel entre les clients et le serveur.
- **Protocoles IRC** : Implémentation des principaux protocoles et commandes IRC pour permettre les connexions, messages, et gestion des utilisateurs et canaux.

## 🎮 Fonctionnalités

- **Connexion et inscription des utilisateurs** : Les utilisateurs peuvent se connecter en fournissant un pseudonyme et participer à des discussions en temps réel.
- **Gestion des canaux** : Création et gestion de canaux de discussion avec des options de mode pour rendre les canaux publics ou privés.
- **Envoi de messages** : Les utilisateurs peuvent envoyer des messages privés ou publics à d'autres utilisateurs ou dans des canaux.
- **Commandes IRC standard** : Support des commandes IRC classiques telles que `/join`, `/part`, `/msg`, `/nick`, et `/quit`.
