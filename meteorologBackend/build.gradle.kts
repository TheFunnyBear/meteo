import org.jetbrains.kotlin.gradle.tasks.KotlinCompile

tasks.wrapper {
	gradleVersion = "8.1.1"
	// You can either download the binary-only version of Gradle (BIN) or
	// the full version (with sources and documentation) of Gradle (ALL)
	distributionType = Wrapper.DistributionType.ALL
}

plugins {
	id("application")
	id("org.springframework.boot") version "2.6.3"
	id("io.spring.dependency-management") version "1.0.11.RELEASE"
	id("org.flywaydb.flyway") version "8.5.9"
	kotlin("jvm") version "1.6.10"
	kotlin("plugin.spring") version "1.6.10"
	kotlin("plugin.jpa") version "1.6.10"
	kotlin("plugin.serialization") version "1.6.10"
}

flyway {
	url = "jdbc:postgresql://localhost:5432/meteorolog"
	user = "admin"
	password = "meteorolog"
}

group = "tema"
version = "0.0.1-SNAPSHOT"

java {
	toolchain {
		languageVersion = JavaLanguageVersion.of(17)
		sourceCompatibility = JavaVersion.VERSION_21
	}
}

repositories {
	mavenCentral()
}

dependencies {
	implementation("org.springframework.boot:spring-boot-starter-data-jpa")
	implementation("org.springframework.boot:spring-boot-starter-web")
	implementation("com.fasterxml.jackson.module:jackson-module-kotlin")
	implementation("org.jetbrains.kotlin:kotlin-reflect")
	implementation("org.jetbrains.kotlinx:kotlinx-serialization-json:1.3.1")
	runtimeOnly("org.postgresql:postgresql")
	testImplementation("org.springframework.boot:spring-boot-starter-test")
	testImplementation("org.jetbrains.kotlin:kotlin-test-junit5")
	testRuntimeOnly("org.junit.platform:junit-platform-launcher")
}

tasks.withType<KotlinCompile> {
	kotlinOptions {
		freeCompilerArgs = listOf("-Xjsr305=strict", "-opt-in=kotlin.RequiresOptIn")
		jvmTarget = "17"
	}
}


allOpen {
	annotation("jakarta.persistence.Entity")
	annotation("jakarta.persistence.MappedSuperclass")
	annotation("jakarta.persistence.Embeddable")
}

tasks.withType<Test> {
	useJUnitPlatform()
}
