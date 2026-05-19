/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

CREATE DATABASE IF NOT EXISTS `hotel` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_as_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `hotel`;

CREATE TABLE IF NOT EXISTS `hospedes` (
  `id` int NOT NULL AUTO_INCREMENT,
  `nome` varchar(255) COLLATE utf8mb4_0900_as_ci NOT NULL,
  `data_nasc` date NOT NULL,
  `cpf` int NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_as_ci;

INSERT INTO `hospedes` (`id`, `nome`, `data_nasc`, `cpf`) VALUES
	(1, 'teste', '2020-01-01', 111111);

CREATE TABLE IF NOT EXISTS `quartos` (
  `id` int NOT NULL AUTO_INCREMENT,
  `descricao` varchar(50) COLLATE utf8mb4_0900_as_ci NOT NULL,
  `valor_diaria` double NOT NULL,
  `tipo_quarto` char(1) CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_as_ci NOT NULL COMMENT 'S = SOLTEIRO, C =CASAL, F = FAMILIA',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_as_ci;

INSERT INTO `quartos` (`id`, `descricao`, `valor_diaria`, `tipo_quarto`) VALUES
	(1, 'Quarto solteiro', 200, 'S'),
	(2, 'Quarto Casal', 300, 'C'),
	(3, 'Quarto Familia', 500, 'F'),
	(4, 'Quarto Familia', 500, 'F');

CREATE TABLE IF NOT EXISTS `reservas` (
  `id` int NOT NULL AUTO_INCREMENT,
  `id_quarto` int NOT NULL DEFAULT '0',
  `id_hospede` int NOT NULL DEFAULT '0',
  `data_inicio` date NOT NULL,
  `data_fim` date NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_as_ci;

INSERT INTO `reservas` (`id`, `id_quarto`, `id_hospede`, `data_inicio`, `data_fim`) VALUES
	(1, 1, 1, '2026-05-18', '2026-05-18'),
	(2, 1, 1, '2020-11-01', '2026-12-01');

/*!40103 SET TIME_ZONE=IFNULL(@OLD_TIME_ZONE, 'system') */;
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IFNULL(@OLD_FOREIGN_KEY_CHECKS, 1) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40111 SET SQL_NOTES=IFNULL(@OLD_SQL_NOTES, 1) */;
